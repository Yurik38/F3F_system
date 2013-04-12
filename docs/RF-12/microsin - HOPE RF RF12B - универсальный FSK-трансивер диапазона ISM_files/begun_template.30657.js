if (typeof Begun !== "object") {
	var Begun = {};
}
Begun.Template = function(tpl) {
	var re;
	tpl = tpl || '';
	this.getTpl = function(){
		return tpl;
	};
	this.REVISION = '$LastChangedRevision: 36491 $';
	this.evaluate = function(vars){
		for (var key in vars) {
			re = new RegExp('\\{\\{' + key + '\\}\\}', 'g');
			tpl = tpl.replace(re, vars[key]);
		}
		// remove unused placeholders
		re = new RegExp('\\{\\{.+?\\}\\}', 'g');
		tpl = tpl.replace(re, '-foo');

		return tpl;
	};
};

if (typeof Begun.Scripts != 'undefined') {
	Begun.Scripts.load("begun_template.js");
}
