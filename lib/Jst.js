/**
 * @fileoverview
 * Javascript templates<br/>
 * see: <a href="http://blog.markturansky.com/BetterJavascriptTemplates.html">http://blog.markturansky.com/BetterJavascriptTemplates.html</a>
 *
 * <p>mschwartz added execute and executeParsed methods.</p>
 */

/*
 Copyright 2008, mark turansky (www.markturansky.com)

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is furnished
 to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 The Software shall be used for Good, not Evil.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.

 (This is the license from www.json.org and I think it's awesome)
 */

function CharacterStack(str){
	this.characters = [];
	this.peek = function peek(){ return this.characters[this.characters.length - 1]; };
	this.pop = function pop() { return this.characters.pop();};
	this.push = function push(c){ this.characters.push(c);};
	this.hasMore =  function hasMore() { if(this.characters.length > 0) { return true; } else { return false; } };

	for(var i = str.length; i >= 0; i = i - 1){
		this.characters.push(str.charAt(i));
	}
}

function StringWriter(){
	this.str = [];
	this.write = function write(s) { this.str.push(s); };
	this.toString = function toString(){ return this.str.join(''); };
}

String.prototype.endsWithChar = function endsWithChar(c){
	if(this.charAt(this.length - 1) == c){
		return true;
	}
	else {
		return false;
	}
};

String.prototype.startsWith = function startsWith(c){
	if(this.charAt(0) == c){
		return true;
	}
	else {
		return false;
	}
};

String.prototype.replaceAll = function replaceAll(a, b){
	var s = this;
	while(s.indexOf(a) > -1){
		s = s.replace(a, b);
	}
	return s;
};

jst_html = [];

/**
 * @class Jst
 */
var Jst = {

//	evalError: function(e, code) {
//		res.reset();
//		res.write('<h1>Template Error</h1>');
//		res.write('<pre>');
//		res.write(e.message + ' at line ' + e.lineNumber + '\n');
//		res.write(e.rhinoException.scriptStackTrace);
//		res.write('</pre>');
//		res.write('<h2>Compiled Template</h2>');
//		res.write('<pre>');
//		var lineNo = 1;
//		forEach(code.split('\n'), function(line) {
//			if (lineNo == e.lineNumber) {
//				res.write(sprintf("<b>%4d %s</b>\n", lineNo++, line.replace(/</, '&lt;')));
//			}
//			else {
//				res.write(sprintf("%4d %s\n", lineNo++, line.replace(/</, '&lt;')));
//			}
//		});
//		res.write('</pre>');
//		Server.abortRequest();
//	},

	parseScriptlet:
		function parseScriptlet(stack){
			var fragment = new StringWriter();
			while(stack.hasMore()){
				if(stack.peek() == '%'){ //possible end delimiter
					var c = stack.pop();
					if(stack.peek() == '>'){ //end delimiter
						// pop > so that it is not available to main parse loop
						stack.pop();
						if(stack.peek() == '\n'){
							fragment.write(stack.pop());
						}
						break;
					} else {
						fragment.write(c);
					}
				} else {
					fragment.write(stack.pop());
				}
			}
			return fragment.toString();
		},

	isOpeningDelimiter:
		function isOpeningDelimiter(c){
			if(c == "<" || c == "%lt;"){
				return true;
			} else {
				return false;
			}
		},

	isClosingDelimiter:
		function isClosingDelimiter(c){
			if(c == ">" || c == "%gt;"){
				return true;
			} else {
				return false;
			}
		},

	appendExpressionFragment:
		function appendExpressionFragment(writer, fragment){
			// check to be sure quotes are on both ends of a string literal
			if(fragment.startsWith("\"") && !fragment.endsWithChar("\"")){
				//some scriptlets end with \n, especially if the script ends the file
				if(fragment.endsWithChar("\n") && fragment.charAt(fragment.length - 2) == '"'){
					//we're ok...
				} else {
					throw { "message":"'" + fragment + "' is not properly quoted"};
				}
			}

			if(!fragment.startsWith("\"") && fragment.endsWithChar("\"")){
				throw { "message":"'" + fragment + "' is not properly quoted"};
			}

			// print or println?
			if(fragment.endsWithChar("\n")){
				writer.write("Jst.writeln(");
				//strip the newline
				fragment = fragment.substring(0, fragment.length - 1);
			} else {
				writer.write("Jst.write(");
			}

			if(fragment.startsWith("\"") && fragment.endsWithChar("\"")){
				//strip the quotes
				fragment = fragment.substring(1, fragment.length - 1);
				writer.write("\"");
				for(var i = 0, len=fragment.length; i < len; i = i + 1){
					var c = fragment.charAt(i);
					if(c == '"'){
						writer.write("\\");
						writer.write(c);
					}
				}
				writer.write("\"");
			} else {
				for(var j = 0, len=fragment.length; j < len; j = j + 1){
					writer.write(fragment.charAt(j));
				}
			}

			writer.write(");\n");
		},

	appendTextFragment:
		function appendTextFragment(writer, fragment){
			if(fragment.endsWithChar("\n")){
				writer.write("Jst.writeln(\"");
			} else {
				writer.write("Jst.write(\"");
			}

			for(var i = 0, len=fragment.length; i < len; ++i){
				var c = fragment.charAt(i);
				if(c == '"'){
					writer.write("\\");
				}
				// we took care of the line break with print vs. println
				if(c != '\n' && c != '\r'){
					writer.write(c);
				}
			}

			writer.write("\");\n");
		},

	parseExpression:
		function parseExpression(stack){
			var fragment = new StringWriter();
			while(stack.hasMore()){
				if(stack.peek() == '%'){ //possible end delimiter
					var c = stack.pop();
					if(Jst.isClosingDelimiter(stack.peek())){ //end delimiter
						//pop > so that it is not available to main parse loop
						stack.pop();
						if(stack.peek() == '\n'){
							fragment.write(stack.pop());
						}
						break;
					} else {
						fragment.write("%");
					}
				} else {
					fragment.write(stack.pop());
				}
			}

			return fragment.toString();
		},

	parseText:
		function parseText(stack){
			var fragment = new StringWriter();
			while(stack.hasMore()){
				if(Jst.isOpeningDelimiter(stack.peek())){ //possible delimiter
					var c = stack.pop();
					if(stack.peek() == '%'){ // delimiter!
						// push c onto the stack to be used in main parse loop
						stack.push(c);
						break;
					} else {
						fragment.write(c);
					}
				} else {
					var d = stack.pop();
					fragment.write(d);
					if(d == '\n'){ //done with this fragment.  println it.
						break;
					}
				}
			}
			return fragment.toString();
		},

	parse:
		function parse(src){
//      src = src.replaceAll("&lt;", "<");
			//src = src.replaceAll("&gt;", ">");

			var stack = new CharacterStack(src);
			var writer = new StringWriter();

			var c;
			var fragment;
			while(stack.hasMore()){
				if(Jst.isOpeningDelimiter(stack.peek())){ //possible delimiter
					c = stack.pop();
					if(stack.peek() == '%'){ //delimiter!
						c = stack.pop();
						if(stack.peek() == "="){
							stack.pop();
							fragment = Jst.parseExpression(stack);
							Jst.appendExpressionFragment(writer, fragment);
						} else {
							fragment = Jst.parseScriptlet(stack);
							writer.write(fragment);
						}
					} else {  //not a delimiter
						stack.push(c);
						fragment = Jst.parseText(stack);
						Jst.appendTextFragment(writer, fragment);
					}
				} else {
					fragment = Jst.parseText(stack);
					Jst.appendTextFragment(writer, fragment);
				}
			}
			return writer.toString();
		},

	write:
		function write(s){
			jst_html.push(s);
		},

	writeln:
		function writeln(s){
			jst_html.push(s + '\n');
		},

	evaluate:
		function evaluate(src, dest, args){
			with(args){
				jst_html = [];
				var script = Jst.parse(document.getElementById(src).value);
				try {
					eval(script);
				}
				catch (e) {
					Jst.evalError(e, script);
				}
				document.getElementById(dest).innerHTML = jst_html.join('');
			}
		},

	include: function(src, args) {
		args.print = Jst.write;
		args.println = Jst.writeln;
		args.pre = function(o, maxRecursion) { Jst.writeln(pre(o, maxRecursion)); };
		with(args){
			var save = jst_html;
			jst_html = [];
			var script = Jst.parse(src);
			try {
				eval(script);
			}
			catch (e) {
				Jst.evalError(e, script);
			}
			var ret = jst_html.join('');
			jst_html = save;
			return ret;
		}
	},

	includeParsed: function(parsed, args) {
		args.print = Jst.write;
		args.println = Jst.writeln;
		args.pre = function(o, maxRecursion) { Jst.writeln(pre(o, maxRecursion)); };
		var save = jst_html;
		jst_html = [];
		try {
			with(args){
				eval(parsed);
			}
		}
		catch (e) {
			Jst.evalError(e, parsed);
		}
		var ret = jst_html.join('');
		jst_html = save;
		return ret;
	},

	execute: function(src, args) {
		//args.templateConstants = res.data.templateConstants;
		args.print = Jst.write;
		args.println = Jst.writeln;
		//args.object = res.data.object;
		jst_html = [];
		var script = Jst.parse(src);
		with(args){
			//try {
			eval(script);
			//}
			//catch (e) {
			//Jst.evalError(e, script);
			//}
		}
		return jst_html.join('');
	},

	executeParsed: function(parsed, args) {
		args.print = Jst.write;
		args.println = Jst.writeln;
		args.pre = function(o, maxRecursion) { Jst.writeln(pre(o, maxRecursion)); };

		jst_html = [];
		with(args){
			eval(parsed);
		}
		return jst_html.join('');
	}
};

