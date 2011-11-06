/**
 * @fileoverview
 * <p>Core class for managing Schemas</p>
 */

/**
 * @class
 *
 * <p>A schema is a JavaScript object that describes a database table.  The metadata
 * about the table fields can be more extensive than what SQL allows.  For example,
 * a field can be marked to be removed from an existing / result object so it can
 * safely be sent to the browser.  Or a function can be provided to validate a field's
 * value.</p>
 *
 * <p>Required members of a schema object are:</p>
 * <ul>
 * <li>name {string} name of schema (and database table)</li>
 * <li>fields {array} array of field definitions (see below)</li>
 * </ul>
 * <p>Optional members of a schema object are:</p>
 * <ul>
 * <li>primaryKey {string} a field name or field names separated by comma</li>
 * <li>engine {string} database engine (defualts to InnoDB)</li>
 * <li>indexes {array} indexes to be created for the table,
 * field name or names separated by comma per index</li>
 * <li>onCreate {function} optional function to call when table is created
 *
 * <p>Required members of a field definition are:</p>
 * <ul>
 * <li>name {string} name of field (database column)</li>
 * <li>type {string} type of field (SQL type, eg. 'int' or 'varchar'...)</li>
 * <li>size {int} required only if type is varchar</li>
 * </ul>
 * <p>Optional members of a field definition are:</p>
 * <ul>
 * <li>defaultValue {object} default value of the field, as when a new record is created</li>
 * <li>autoIncrement {boolean} true if this is an auto_increment field</li>
 * <li>ALL OTHER members are ignored at this time</li>
 * </ul>
 * <p>An example Schema definition:</p>
 * <pre>
 * 	Schema.add({
 *		name: 'UserGroups',
 *		fields: [
 *			{ name: 'userGroupId', type: 'int', autoIncrement: true, defaultValue: 0 },
 *			{ name: 'groupName', type: 'varchar', size: 64, defaultValue: '' },
 *			{ name: 'isAdmin', type: 'tinyint', size: 1, defaultValue: 0 }
 *		],
 *		primaryKey: 'userGroupId',
 *		engine: 'InnoDB',
 *		onCreate: function() {
 *			Schema.putOne('UserGroups', {
 *				userGroupId: 0,
 *				groupName: 'Administrators',
 *				isAdmin: 1
 *			});
 *			Schema.putOne('UserGroups', {
 *				userGroupId: 0,
 *				groupName: 'Standard',
 *				isAdmin: 0
 *			});
 *		}
 *	});
 * </pre>
 */
Schema = function() {
	"use strict";

	var schemas = {};
	var onStartFuncs = [];

	/** @private */
	function getSchema(name) {
		if (Util.isString(name)) {
			var schema = schemas[name];
			if (!schema) {
				throw 'No such schema ' + name;
			}
			return schema;
		}
		return name;
	}

	/**
	 * @private
	 *
	 * get an appropriate defaultValue for given field
	 * if defaultValue present in field, use it
	 * otherwise return something appropriate to the field's type
	 */
	function defaultValue(field) {
		if (field.defaultValue) {
			if (Util.isFunction(field.defaultValue)) {
				return field.defaultValue();
			}
			return field.defaultValue;
		}
		switch (field.type) {
			case 'int':
			case 'tinyint':
				return 0;
			default:
				return '';
		}
	}

	/**
	 * @private
	 * Query a Schema for a row or rows by example
	 *
	 * @param {object} name name of Schema OR a Schema object
	 * @param {object} example example to query
	 * @param {boolean} single true to return a single row, otherwise all matching rows
	 * @returns {object} A single row matching example, or all matching rows
	 */
	function find(name, example, single) {
		var schema = getSchema(name);
		name = schema.name;
		example = example || {};
		var where = Schema.where(name, example);
		var query = [
			'SELECT',
			'       *',
			'FROM',
			'       ' + name
		];
		if (where.length) {
			query.push('WHERE');
			query.push(where.join(' AND '));
		}
		if (single) {
			var ret = SQL.getDataRow(query);
			return empty(ret) ? false : Schema.onLoad(schema, ret);
		}
		else {
			return Schema.onLoad(schema, SQL.getDataRows(query));
		}
	}

	/** @private **/
	// onCreate functions are called at onStart time to assure all the tables
	// are created first.
	function onStart() {
		forEach(onStartFuncs, function(func) {
			func();
		});
	}

	Server.addOnStart({name: 'Schemas', func: onStart});

	/** @scope Schema */
	return {
		/**
		 * <p>Get all schemas in ExtJS DataStore/Record format.</p>
		 */
		getSchemaExtJs: function() {
			var extjs = {};
			forEach(schemas, function(schema) {
				var fields = [];
				forEach(schema.fields, function(field) {
					if (!field.serverOnly) {
						switch (field.type) {
							case 'int':
							case 'tinyint':
								fields.push({ name: field.name, type: 'int '});
								break;
							default:
								fields.push({ name: field.name });
						}
					}
				});
				extjs[schema.name] = fields;
			});
			return extjs;
		},


		/**
		 * <p>Add a schema</p>
		 *
		 * <p>If the database table for the schema does not exist, it is
		 * created.</p>
		 *
		 * <p>TODO handle diff between existing dataabase table and schema
		 * passed in.</p>
		 *
		 * @param {object} schema schema definition
		 * @returns {void} nothing
		 */
		add: function(schema) {
			var name = schema.name;
			schemas[name] = schema;

			// this try/catch determines if the table exists in the database
			try {
				SQL.getDataRow('SHOW CREATE TABLE ' + name);
				Schema.change(name);
			}
			catch (e) {
				// doesn't exist, create it
				Schema.create(name);
			}
		},

		/**
		 * <p>Define an abstract schema</p>
		 *
		 * <p>Abstract schemas are meant to be extended to create a schema
		 * that is represented in the database.</p>
		 *
		 * @see Schema#extend
		 *
		 * @param {object} schema abstract schema definition
		 */
		define: function(schema) {
			var name = schema.name;
			schemas[name] = schema;
		},

		/**
		 * <p>Extend (inherit from) an existing schema</p>
		 *
		 * <p>Typically an abstract schema is extended.</p>
		 *
		 * @param {object} name name of base schema (or schema) to extend
		 * @param {object} child fields to extend base schema with
		 * @returns {void} nothing
		 */
		extend: function(name, child) {
			var schema = getSchema(name);
			// TODO don't concat reserved fields
			child.fields = schema.fields.concat(child.fields);
			child.primaryKey = schema.primaryKey;
			if (child.indexes && schema.indexes) {
				child.indexes = schema.indexes.concat(child.indexes);
			}
			Schema.add(child);
		},

		/**
		 * Determine if database table for a schema exists
		 *
		 * @param {string} name name of database table (and schema)
		 * @returns {boolean} true if table exists, false otherwise
		 */
		exists: function(name) {
			try {
				SQL.getDataRow('SHOW CREATE TABLE ' + name);
				return true;
			}
			catch (e) {
				return false;
			}
		},

		/**
		 * Get a schema by name
		 *
		 * @param name name of schema
		 */
		getSchema: function(name) {
			return getSchema(name);
		},

		/**
		 * Get an empty/new record for a schema
		 * Merges in an example, if provided.
		 *
		 * @param {object} name name of schema or schema proper
		 * @param {object} example optional example to merge
		 * @returns {object} record with default values
		 */
		newRecord: function(name, example) {
			var schema = getSchema(name);
			var record = {};
			forEach(schema.fields, function(field) {
				if (!field.reserved) {
					record[field.name] = defaultValue(field);
				}
			});
			return Util.apply(record, example || {});
		},

		/**
		 * Clean a record.
		 * Removes fields marked clean: true in the schema from the record.
		 *
		 * @param {object} record record to clean
		 * @returns {object} record with fields removed
		 */
		clean: function(name, record) {
			var schema = getSchema(name);
			forEach(schema.fields, function(field) {
				if (field.serverOnly) {
					delete record[field.name];
				}
			});
			return record;
		},

		/**
		 * <p>Generate a WHERE clause for SQL query based upon an example.
		 * An array of "table.key=value" strings is returned, which can be concatonated
		 * with another result of this function to generate WHERE clauses for JOIN
		 * type queries.  The values are SQL quoted proper.  If a value contains a %,
		 * then LIKE is generated.</p>
		 *
		 * @param {object} name name of schema or schema proper
		 * @param {object} example example to generate WHERE clause for
		 * @return {object} array of "table.key=value"
		 */
		where: function(name, example) {
			var schema = getSchema(name);
			name = schema.name;
			var where = [];
			forEach(schema.fields, function(field) {
				if (!field.noQuery && !field.reserved && example[field.name] !== undefined) {
					var value = example[field.name];
					if (Util.isString(value) && value.indexOf('%') !== -1) {
						where.push(['   ',name,'.',field.name,' LIKE ',SQL.quote(value)].join(''));
					}
					else if (Util.isArray(value)) {
						where.push(['	',name,'.',field.name,' IN (',SQL.quote(value),')'].join(''));
					}
					else {
						where.push(['   ',name,'.',field.name,'=',SQL.quote(value)].join(''));
					}
				}
			});
			return where;
		},

		/**
		 * Return count of records in schema based on example
		 *
		 * @param {object} name name of schema or schema
		 * @param {object} example example to count
		 * @returns {int} count of records matching example
		 */
		count: function(name, example) {
			var schema = getSchema(name);
			name = schema.name;
			example = example || {};
			var where = Schema.where(name, example);
			var query = [
				'SELECT',
				'       COUNT(*)',
				'FROM',
				'       ' + name
			];
			if (where.length) {
				query.push('WHERE');
				query.push(where.join(' AND '));
			}
			return SQL.getScalar(query);
		},

		onLoad: function(name, example) {
			var schema = getSchema(name);
			if (!Util.isArray(example)) {
				schema.onLoad && schema.onLoad(example);
			}
			else {
				forEach(example, function(ex) {
					schema.onLoad && schema.onLoad(ex);
				});
			}
			return example;
		},

		onPut: function(name, example) {
			var schema = getSchema(name);
			if (!Util.isArray(example)) {
				schema.onPut && schema.onPut(example);
			}
			else {
				forEach(example, function(ex) {
					schema.onPut && schema.onPut(ex);
				});
			}
			return example;
		},

		/**
		 *	Query a schema for multiple records by example
		 *
		 *	@param {object} name name of a schema or a schema proper
		 *	@param {object} example example to query
		 *	@returns {object} all matching rows from the database
		 */
		find: function(name, example) {
			return find(name, example, false);
		},

		/**
		 *	Query a schema for a single record by example
		 *
		 *	@param {object} name name of a schema or a schema proper
		 *	@param {object} example example to query
		 *	@returns {object} a single row from the database
		 */
		findOne: function(name, example) {
			return find(name, example, true);
		},

		/**
		 * <p>Get a list for ExtJS grid</p>
		 *
		 * @param {object} name name of schema or schema struct
		 * @param {object} example
		 * @param {function} fn optional function called to add information to each record
		 * @returns {object} object suitable for sending as JSON for ExtJS DataStores.
		 *
		 * <p>Note: if fn is provided, it will be called for each record in the returned list with
		 * the record as the only argument.</p>
		 */
		list: function(name, example, fn) {
			var schema = getSchema(name);
			name = schema.name;

			var startRow = req.data.start || 0;
			var maxRows = req.data.limit || 25;
			var sort = req.data.sort || schema.primaryKey;
			var dir = req.data.dir || 'ASC';

			var where = Schema.where(name, example);
			var query = [
				'SELECT',
				'       COUNT(*)',
				'FROM',
				'       ' + name
			];
			if (where.length) {
				query.push('WHERE');
				query.push(where.join(' AND '));
			}
			var count = SQL.getScalar(query);
			query[1] = '	*';
			query = query.concat([
				'ORDER BY',
				'	' + sort + ' ' + dir,
				'LIMIT',
				'	' + startRow + ',' + maxRows
			]);
			var items = SQL.getDataRows(query);
			fn && forEach(items, fn);
			return {
				count: count,
				list: items
			};
		},

		/**
		 * <p>Store a record in the database</p>
		 *
		 * <p>This function will insert a new record or update an existing record.</p?
		 *
		 * <p>If the record is new, the example is merged with default values so a complete
		 * record is created.  The primary key in the returned record is set to the insert_id
		 * generated by the query.</p>
		 *
		 * <p>If the record exists, the example is merged with the existing record and updated
		 * in the database.</p>
		 *
		 * @param {object} schema name of schema or schema proper
		 * @param {object} example full or partial record to store in the database
		 * @returns {object} complete record as stored in the database.
		 */
		putOne: function(schema, example) {
			schema = getSchema(schema);
			var name = schema.name;
			var primaryKey = (schema.primaryKey && schema.primaryKey.indexOf(',') === -1) ? schema.primaryKey : undefined;

			example = Schema.newRecord(schema, example);
			example = Schema.onPut(schema, example);
			var keys = [], values = [];
			forEach(schema.fields, function(field) {
				if (!field.reserved) {
					keys.push(field.name);
					values.push(SQL.quote(example[field.name]));
				}
			});
			SQL.update('REPLACE INTO ' + name + ' (' + keys.join(',') + ') VALUES (' + values.join(',') + ')');
			if (primaryKey && !example[primaryKey]) {
				example[primaryKey] = SQL.insertId();
			}
			return Schema.onLoad(schema, example);
		},

		/**
		 * <p>Remove one or more records from the database.</p>
		 *
		 * @param {string} name name of schema or schema proper.
		 * @param {object} example example of record(s) to remove from database.
		 * @returns {int} number of rows removed
		 */
		remove: function(name, example) {
			var where = Schema.where(name, example);
			if (!where.length) {
				throw 'Invalid example provided to remove function';
			}
			example = Schema.onPut(example);
			var query = [
				'DELETE',
				'FROM',
				'       ' + name
			];
			query.push('WHERE');
			query.push(where.join(' AND '));
			return SQL.update(query);
		},

		/**
		 * Create a database table from a schema
		 *
		 * @param {string} name name of schema or schema proper
		 * @param {boolean} drop (optional) true to first drop table
		 * @returns {void} nothing
		 */
		create: function(name, drop) {
			var schema = getSchema(name);
			if (drop) {
				SQL.update('DROP TABLE IF EXISTS ' + schema.name);
			}
			var query = [
				'CREATE TABLE ' + name + ' ('
			];
			var primaryKey = null;
			forEach(schema.fields, function(field) {
				if (!field.reserved) {
					if (field.autoIncrement) {
						primaryKey = field.name;
						query.push('	' + field.name + ' ' + field.type + ' auto_increment,');
					}
					else if (field.type === 'varchar') {
						query.push('	' + field.name + ' varchar(' + field.size + '),');
					}
					else {
						query.push('	' + field.name + ' ' + field.type + ',');
					}
				}
			});
			forEach(schema.indexes, function(index) {
				query.push('	INDEX(' + index + '),');
			});
			if (schema.primaryKey) {
				query.push('	Primary Key(' + schema.primaryKey + ')');
			}
			else if (primaryKey) {
				query.push('	Primary Key(' + primaryKey + ')');
			}
			else {
				var len = query.length - 1;
				query[len] = query[len].replace(/,$/, '');
			}
			var engine = schema.engine || 'InnoDB';
			query.push(') Engine=' + engine);
			SQL.update(query);
			if (schema.onCreate) {
				onStartFuncs.push(schema.onCreate);
				//schema.onCreate();
			}
		},

		/**
		 * <p>Generate a Schema definition from an existing database table.</p>
		 *
		 * <p>TODO this function could fill in initialRecords with all the records
		 * in the existing table.</p>
		 *
		 * @param {string} name of database table
		 * @returns {object} Schema definition
		 */
		getFromTable: function(name) {
			var schema = {
				name: name
			};
			var fields = [];
			var rows = SQL.getDataRows('DESCRIBE ' + name);
			forEach(rows, function(row) {
				var unsigned = row.Type.indexOf('unsigned') != -1;
				var enumVals = null;
				var type = row.Type.replace(/\s+unsigned/, '');
				var size = type.replace(/^.*\(/, '').replace(/\).*$/, '');
				type = type.replace(/\(.*\)/, '');
				switch (type.toLowerCase()) {
					case 'text':
					case 'blob':
						size = 65535;
						break;
					case 'mediumtext':
					case 'mediumblob':
						size = 16777216;
						break;
					case 'longtext':
					case 'longblob':
						size = 4294967296;
						break;
					case 'enum':
						enumVals = size.split(',');
						size = 1;
						break;
				}
				if (row.Extra === 'auto_increment') {
					fields.push({
						name: row.Field,
						type: type,
						size: size,
						autoIncrement: true,
						defaultValue: row.Default
					});
				}
				else if (enumVals) {
					fields.push({
						name: row.Field,
						type: type,
						size: size,
						enumVals: enumVals,
						defaultValue: row.Default
					});
				}
				else {
					fields.push({
						name: row.Field,
						type: type,
						size: size,
						defaultValue: row.Default
					});
				}
			});
			schema.fields = fields;
			rows = SQL.getDataRows('SHOW INDEXES IN ' + name);
			var indexes = {};
			forEach(rows, function(row) {
				indexes[row.Key_name] = indexes[row.Key_name] || [];
				indexes[row.Key_name].push(row.Column_name);
			});
			var indexArray = [];
			forEach(indexes, function(index, key) {
				if (key === 'PRIMARY') {
					schema.primaryKey = index.join(',');
				}
				else {
					indexArray.push(index.join(','));
				}
			});
			if (indexArray.length) {
				schema.indexes = indexArray;
			}
			return schema;
		},

		change: function(name) {
			var schema = getSchema(name);

			// table exists, alter the table if necessary
			var existing = Schema.getFromTable(name);

			function fieldTypeCompare(srcField, dstField) {
				if (srcField.type === dstField.type) {
					if (srcField.type === 'varchar' && srcField.size == dstField.size) {
						return true;
					}
					else if (srcField.type == 'int') {
						if (srcField.autoIncrement == dstField.autoIncrement) {
							return true;
						}
					}
					else {
						return true;
					}
				}
				return false;
			}

			function changeField(srcField, dstField) {
				var query = [
					'ALTER TABLE',
					'	'+name,
					'CHANGE'
				];
				if (srcField.autoIncrement) {
					query.push('	' + dstField.name + ' ' + srcField.name + ' ' + srcField.type + ' auto_increment');
				}
				else if (srcField.type == 'varchar') {
					query.push('	' + dstField.name + ' ' + srcField.name + ' varchar(' + srcField.size + ')');
				}
				else {
					query.push('	' + dstField.name + ' ' + srcField.name + ' ' + srcField.type);
				}
				SQL.update(query);
			}

			function addField(field) {
				var query = [
					'ALTER TABLE',
					'	'+name,
					'ADD'
				];
				if (field.autoIncrement) {
					query.push('	' + field.name + ' ' + field.type + ' auto_increment');
				}
				else if (field.type == 'varchar') {
					query.push('	' + field.name + ' varchar(' + field.size + ')');
				}
				else {
					query.push('	' + field.name + ' ' + field.type);
				}
				SQL.update(query);
			}

			// index source fields
			var srcFields = {};
			forEach(schema.fields, function(field) {
				if (!field.reserved) {
					srcFields[field.name] = field;
				}
			});

			// index destination fields
			var dstFields = {};
			forEach(existing.fields, function(field) {
				dstFields[field.name] = field;
			});
			function findDstField(srcField) {
				for (var i in dstFields) {
					if (fieldTypeCompare(srcField, dstFields[i])) {
						return dstFields[i];
					}
				}
				return false;
			}

			var fieldsProcessed = {};
			// match up schema fields with existing fields and process them
			forEach(srcFields, function(srcField) {
				var dstField = dstFields[srcField.name];
				if (dstField) {
					// there is a field in the schema with same name as in the database
					if (srcField.type !== dstField.type) {
						// need to alter the table/field type
						changeField(srcField, dstField);
					}
					delete dstFields[srcField.name];
					fieldsProcessed[srcField.name] = srcField;
				}
			});

			// look at remaining schema fields to see if existing fields are being renamed
			forEach(srcFields, function(srcField) {
				if (fieldsProcessed[srcField.name]) {
					return;
				}
				var dstField = findDstField(srcField);
				if (dstField) {
					// rename existing field
					changeField(srcField, dstField);
					delete dstFields[dstField.name];
					fieldsProcessed[srcField.name] = srcField;
				}
			});

			// remove remaining destination fields
			forEach(dstFields, function(dstField) {
				SQL.update([
					'ALTER TABLE',
					'	' + name,
					'DROP',
					'	' + dstField.name
				]);
			});

			// add any remaining source fields
			forEach(srcFields, function(srcField) {
				if (fieldsProcessed[srcField.name]) {
					return;
				}
				addField(srcField);
			});

			// todo handle indexes
		}

	};
}();
