/**
 * Created by JetBrains WebStorm.
 * User: mschwartz
 * Date: 2/14/12
 * Time: 5:41 PM
 * To change this template use File | Settings | File Templates.
 */

include('lib/coffee-script.js');
include('lib/print_r.js');
include('lib/forEach.js');
include('lib/Exceptions.js');
include('./main.coffee');

global.main_action = function() {
	res.write('hello coffee world')
		res.stop()
}
