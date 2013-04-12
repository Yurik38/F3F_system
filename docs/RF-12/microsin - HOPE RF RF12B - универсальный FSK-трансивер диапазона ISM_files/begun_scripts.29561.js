if (typeof Begun !== "object") {
	var Begun = {};
}
if (typeof Begun.Scripts !== "object") {
	Begun.Scripts = {};
}
(function() {
if (typeof Begun.importedScripts === "undefined") {
	Begun.Scripts.Callbacks = {};
	Begun.importedScripts   = [];
	var i;
	var mixin = {
		REVISION: '$LastChangedRevision: 36491 $',
		_required: {},
		_toRun: [],
		_baseUrl: window.begun_urls&&window.begun_urls.base_scripts_url?window.begun_urls.base_scripts_url:"http://autocontext.begun.ru/",
		_norm: function(scriptUrl, isStrict) {
			if (!scriptUrl) {
				return;
			}
			if ((scriptUrl.indexOf(this._baseUrl) !== 0) && (scriptUrl && scriptUrl.indexOf("http") === -1)) {
				scriptUrl = this._baseUrl + scriptUrl.replace(/\?.*$/, '');
			}
			if (!isStrict) {
				return scriptUrl;
			}
			return scriptUrl.replace(/acp\/([^.]+).[^.]*.js/,"$1.js");
		},
		_importScript: function(scriptUrl) {
			scriptUrl = this._norm(scriptUrl);
			if (Begun.importedScripts.toString().indexOf(scriptUrl) !== -1) {
				return;
			}
			Begun.importedScripts.push(scriptUrl);
			document.write("<scr" + "ipt type=\"text/javascript\" src=\"" + scriptUrl + "\"></scr" + "ipt>");
		},
		_require: function(requiredScripts) {
			for (var reqScript in requiredScripts) {
				if (requiredScripts[reqScript] && requiredScripts.hasOwnProperty(reqScript)
					&& Begun.importedScripts.toString().indexOf(this._norm(reqScript)) === -1) {
					this._required[this._norm(reqScript)] = requiredScripts[reqScript];
				}
			}
		},
		importAllScripts: function(scriptUrls) {
			this._require(scriptUrls);
			for (var reqScript in scriptUrls) {
				if (scriptUrls[reqScript] && scriptUrls.hasOwnProperty(reqScript)) {
					this._importScript(reqScript);
				}
			}
		},
		isLastRequired: function(scriptUrl) {
			var numb = 0;
			for (var reqScript in this._required) {
				if (this._required[reqScript] && this._required.hasOwnProperty(reqScript)) {
					if (this._norm(reqScript, true) === this._norm(scriptUrl, true)) {
						delete this._required[reqScript];
					} else {
						numb++;
					}
				}
			}
			return (numb < 1);
		},
		load: function(fileName) {
			this.isLastRequired(fileName);
			for (var i = 0; i < this._toRun.length; i++) {
				this._toRun[i](fileName);
			}
		},
		addStrictFunction: function(newOne) {
			for (var i = 0; i < this._toRun.length; i++) {
				if (this._toRun[i].toString() === newOne.toString()) {
					return;
				}
			}
			this._toRun.push(newOne);
			this.load();
		}
	};
	for (i in mixin) {
		if (mixin.hasOwnProperty(i)) {
			Begun.Scripts[i] = mixin[i];
		}
	}
}
	if (typeof Begun.loaderCallbacks !== 'undefined') {
		for (i = 0; i < Begun.loaderCallbacks.length; i++) { 
			Begun.loaderCallbacks[i]();
		}
	}
})()

