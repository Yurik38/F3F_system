if (typeof Begun !== "object") {
	var Begun = {};
}
if (!Begun.Utils) {
	Begun.$ = function(id) {
		return document.getElementById(id);
	};
	Begun.extend = function(destination, source) {
		for (var property in source) {
			destination[property] = source[property];
		}
		return destination;
	};

	Begun.Browser = new function() {
		var _ua = navigator.userAgent;
		this.IE = !!(window.attachEvent && _ua.indexOf('Opera') === -1);
		this.Opera =  _ua.indexOf('Opera') > -1;
		this.WebKit = _ua.indexOf('AppleWebKit/') > -1;
		this.Gecko =  _ua.indexOf('Gecko') > -1 && _ua.indexOf('KHTML') === -1;
		var ver = null;
		this.version = function() {
			if (ver !== null){
				return ver;
			}
			if (typeof detect !== "undefined") {
				ver = detect();
				return ver;
			} else {
				return;
			}
		};
		var detect;
		function check(re){
			var versionParsed = re.exec(_ua);
			if (versionParsed && versionParsed.length && versionParsed.length > 1 && versionParsed[1]) {
				return versionParsed[1];
			}
		}
		if (this.IE){
			detect = function() { return check(/MSIE (\d*.\d*)/gim); };
		} else if (this.Opera) {
			detect = function() { return check(/Opera\/(\d*\.\d*)/gim); };
		} else if (this.WebKit) {
			detect = function() { return check(/AppleWebKit\/(\d*\.\d*)/gim); };
		} else if (this.Gecko) {
			detect = function() { return check(/Firefox\/(\d*\.\d*)/gim); };
		}
		this.more = function(n) {
			return parseFloat(this.version()) > n;
		};
		this.less = function(n) {
			return parseFloat(this.version()) < n;
		};
	}();

	Begun.Utils = new function() {
		var d = document;
		var script_count = 0;
		var swf_count = 0;
		var script_timeout = 5000;
		var getHead = function(obj) {
			var wnd = obj || window;
			var head = wnd.document.getElementsByTagName("head")[0];
			if (!head){
				head = wnd.document.createElement("head");
				wnd.document.documentElement.insertBefore(head, wnd.document.documentElement.firstChild);
			}
			return head;
		};
		this.REVISION = '$LastChangedRevision: 36491 $';


		this.includeScript = function(url, type, callback, callback_name, check_var){
			var INC_SCRIPT_PREFIX = 'begun_js_';
			var SCRIPT_TYPE = 'text/javascript';
			var scriptTimeoutCounter = "http://autocontext.begun.ru/blockcounter?pad_id={{pad_id}}&block={{block_id}}&script_timeout=1";
			type = type || 'write'; // append or write
			var inc = 0;
			var script = null;
			if (url){
				if (type == 'write'){
					script_count++;
					var id = INC_SCRIPT_PREFIX + script_count;
					if (check_var){
						window.setTimeout(function(){
							var scripts = d.getElementsByTagName("script");
							var s = scripts.length > 0 ? scripts[scripts.length - 1] : null;
							if (s && window[check_var] === undefined){
								s.parentNode.removeChild(s);
								Begun.Utils.includeCounter(scriptTimeoutCounter, {'pad_id': (window.begun_auto_pad || ''), 'block_id': (window.begun_block_id || '')});
							}
						}, script_timeout);
					}
					d.write('<scr'+'ipt type="' + SCRIPT_TYPE + '" src="' + url + '" id="' + id + '"></scr'+'ipt>'); 
					script = Begun.$(id);
				} else if (type == 'append'){
					script = d.createElement("script");
					script.src = url;
					script.type = SCRIPT_TYPE;
					var head = getHead();
					head.appendChild(script);
				}
				if (script && typeof callback == 'function'){
					var callback_fired = false;
					script.onload = function(){
						if (!callback_fired){
							callback();
							callback_fired = true;
						}
					};
					script.onreadystatechange = function(){
						if (callback_fired){
							return;
						}
						var check_statement = (Begun.Browser.Opera ? (this.readyState == "complete") : (this.readyState == "complete" || this.readyState == "loaded"));
						if (check_statement){
							callback();
							callback_fired = true;
						}
					};
				}
			}
		};
		this.evalScript = function(code){
			try {
				eval(code);
			} catch(e) {}
		};
		this.checkFlash = function() {
			for (var i=3;i<10;i++){
				var string = 'ShockwaveFlash.ShockwaveFlash.'+i;
				try{
					var obj = new ActiveXObject(string);
					if(obj) {
						return true;
					}
				} catch (e) {}
			}
			if(navigator.mimeTypes && navigator.mimeTypes.length && navigator.mimeTypes['application/x-shockwave-flash'] && navigator.mimeTypes['application/x-shockwave-flash'].enabledPlugin) {
				return true;
			} else if(navigator.plugins["Shockwave Flash"]) {
				return true;
			} else {
				return false;
			}
		};
		this.reformSWFData = function(url){
			var params = url.split(/\?/);
			if(params[1] && params[1].match(/uuid/)) {
				var data = params[1].split(/&/);
				var reformed_params = '';
				for(var i=0, l=data.length; i<l; i++) {
					var tmp_data = data[i].split(/=/);
					if(tmp_data[1]) {
						reformed_params += encodeURIComponent(tmp_data[0]) + ':' + encodeURIComponent(tmp_data[1]);
						if(i != l-1) {
							reformed_params += ',';
						}
					}
				}
				var postback_url = params[0].replace('.swf', '.xml');
				url = params[0] + '?sync_params=' + reformed_params + '&postback_url=' + encodeURIComponent(postback_url);
			}
			return url;
		};
		this.includeSWF = function(url){
			url = this.reformSWFData(url);
			var INC_SWF_PREFIX = 'begun_swf_';
			swf_count++;
			var swf_wrapper = d.createElement('div');
			swf_wrapper.style.visability = 'hidden';
			swf_wrapper.style.top = '-1000px';
			swf_wrapper.style.left = '-1000px';
			swf_wrapper.innerHTML =
				'<object id="' + INC_SWF_PREFIX + swf_count + '" type="application/x-shockwave-flash" data="' + url + '" width="1" height="1">' + 
					'<param name="movie" value="' + url + '" />' + 
				'</object>';
			d.getElementsByTagName('body') && d.getElementsByTagName('body')[0] && d.getElementsByTagName('body')[0].appendChild(swf_wrapper);
		};
		this.includeStyle = function(css_text, type, id, wnd){
			wnd = wnd || window;
			type = type || 'write'; // append or write
			var DEFUALT_STYLE_ID = 'begun-default-css';
			var style;
			id = id || DEFUALT_STYLE_ID;
			if (css_text){
				if (type == 'write'){
					wnd.document.write('<style type="text/css" id="' + id + '">' + css_text + '</style>');
				} else if (type == 'append'){
					// IE
					if (wnd.document.createStyleSheet){
						style = null;
						try{
							style = wnd.document.createStyleSheet();
							style.cssText += css_text;
						}catch(e){
							for (var i = wnd.document.styleSheets.length - 1; i >= 0; i--){
								try{
									style = wnd.document.styleSheets[i];
									style.cssText += css_text;
									break; // access granted? get outta here!
								}catch(k){
									style = null;
								}
							}
						}
					} else {
						if (Begun.$(id)) {
							// Для Google Chrome 4 (и ниже) исключение (#5385). Для GC 5 beta всё уже ok
							if (Begun.Browser.WebKit && Begun.Browser.version() < 533) {
								Begun.$(id).appendChild(document.createTextNode(css_text));
							} else {
								Begun.$(id).innerHTML = css_text;
							}
						} else {
							style = wnd.document.createElement("style");
							style.setAttribute("type", "text/css");
							style.id = id;
							style.appendChild(wnd.document.createTextNode(css_text));
							getHead(wnd).appendChild(style);
						}
					}
				}
			}
		};
		this.includeCSSFile = function(url){
			var style = d.createElement("link");
			style.setAttribute("type", "text/css");
			style.setAttribute("rel", "stylesheet");
			style.href = url;
			getHead().appendChild(style);
		};
		this.includeCounter = function(src, obj) {
			var re;
			if(!src || !obj) {
				return;
			}
			for (var key in obj) {
				re = new RegExp('\\{\\{' + key + '\\}\\}', 'g');
				src = src.replace(re, obj[key]);
			}
			re = /\{\{.+?\}\}/g;
			src = src.replace(re, '-foo');
			(new Image()).src = src;
		};
		this.toQuery = function(params) {
			var result = '';
			var ampersand = '';
			var toTail = {
				'real_refer': false,
				'ref': false
			};
			var key;
			for (key in params) {
				if (params[key] && params.hasOwnProperty && params.hasOwnProperty(key)) {
					if (typeof toTail[key] !== "undefined") {
						toTail[key] = encodeURIComponent(params[key]);
					} else {
						result += ampersand + key + '=' + encodeURIComponent(params[key]);
						ampersand = '&';
					}
				}
			}
			for (key in toTail) {
				if (toTail[key] && toTail.hasOwnProperty && toTail.hasOwnProperty(key)) {
					result += ampersand + key + '=' + toTail[key];
				}
			}
			return result;
		};
		this.toJSON = function(obj){
			switch (typeof obj) {
				case 'object':
					if (obj) {
						var list = [];
						if (obj instanceof Array) {
						for (var i=0;i < obj.length;i++) {
							list.push(this.toJSON(obj[i]));
						}
						return '[' + list.join(',') + ']';
						} else {
							for (var prop in obj) {
								list.push('"' + prop + '":' + this.toJSON(obj[prop]));
							}
							return '{' + list.join(',') + '}';
						}
					} else {
						return 'null';
					}
				case 'string':
					return '"' + obj.replace(/(["'])/g, '\\$1') + '"';
				case 'number':
					return obj;
				case 'boolean':
					return new String(obj);
			}
		};
		this.in_array = function(arr, value){
			for (var i = 0, l = arr.length; i < l; i++){
					if (arr[i] == value){
						return true;
					}
			}
			return false;
		};
		this.inList = function(string, value){
			var arr = string && string.toLowerCase().split(',');
			if(!arr) {
				return false;
			} else {
				return Begun.Utils.in_array(arr, value.toLowerCase());
			}
		};
		this.countWindowSize = function() {
			var w = 0, h = 0;
			if( typeof( window.innerWidth ) == 'number' ) {
				w = window.innerWidth;
				h = window.innerHeight;
			} else if( d.documentElement && ( d.documentElement.clientWidth || d.documentElement.clientHeight ) ) {
				w = d.documentElement.clientWidth;
				h = d.documentElement.clientHeight;
			} else if( d.body && ( d.body.clientWidth || d.body.clientHeight ) ) {
				w = d.body.clientWidth;
				h = d.body.clientHeight;
			}
			var obj = {
				width: w,
				height: h
			};
			return obj;
		};
		this.getScrollXY = function () {
			var x = 0, y = 0;
			if( typeof( window.pageYOffset ) == 'number' ) {
				y = window.pageYOffset;
				x = window.pageXOffset;
			} else if( d.body && ( d.body.scrollLeft || d.body.scrollTop ) ) {
				y = d.body.scrollTop;
				x = d.body.scrollLeft;
			} else if( d.documentElement && ( d.documentElement.scrollLeft || d.documentElement.scrollTop ) ) {
				y = d.documentElement.scrollTop;
				x = d.documentElement.scrollLeft;
			}
			var obj = {
				x: x,
				y: y
			};
			return obj;
		};
		this.findPos = function(obj) {
			var curleft = 0;
			var curtop = 0;
			if (obj && obj.offsetParent) {
				do {
					curleft += obj.offsetLeft;
					curtop += obj.offsetTop;
				} while (obj = obj.offsetParent);
				obj = {
					left: curleft,
					top: curtop
				};
				return obj;
			}
		};
		this.addEvent = function (obj,name,func) {
			if(obj.addEventListener) {
				obj.addEventListener(name, func, false);
			} else if (obj.attachEvent) {
				obj.attachEvent('on'+name, func);
			}
		};
		this.hasClassName = function(element, className) {
			return new RegExp("(^|\\s)" + className + "(\\s|$)").test(element.className);
		};
		this.addClassName = function(element, className) {
			if (!this.hasClassName(element, className)) {
				element.className += (element.className ? ' ' : '') + className;
			}
				return element;
		};
		this.removeClassName = function(element, className) {
			if (this.hasClassName(element,className)) {
				var reg = new RegExp('(\\s|^)'+className+'(\\s|$)');
				element.className=element.className.replace(reg,' ');
			}
		};
		this.toCamelCase = function(s){
			for(var exp=/-([a-z])/; exp.test(s); s=s.replace(exp,RegExp.$1.toUpperCase())) {}
			return s;
		};
		this.getStyle = function(el, a) {
			if (!el) {
				return;
			}
			el = el || Begun.$(el); 
			var v = null;
			if(d.defaultView && d.defaultView.getComputedStyle){
				var cs = d.defaultView.getComputedStyle(el,null);
				if(cs && cs.getPropertyValue) {
					v = cs.getPropertyValue(a);
				}
			}
			if(!v && el && el.currentStyle) {
				v = el.currentStyle[this.toCamelCase(a)];
			}
			return v;
		};
		this.getElementsByClassName = function(oElm, strTagName, strClassName){
			var arrElements = (strTagName == "*" && oElm.all)? oElm.all : oElm.getElementsByTagName(strTagName);
			var arrReturnElements = new Array();
			strClassName = strClassName.replace(/\-/g, "\\-");
			var oRegExp = new RegExp("(^|\\s)" + strClassName + "(\\s|$)");
			var oElement;
			for (var i=0; i<arrElements.length; i++) {
				oElement = arrElements[i];     
				if (oRegExp.test(oElement.className)){
					arrReturnElements.push(oElement);
				}   
			}
			return (arrReturnElements);
		};
		this.unescapeHTML = function(txt) {
			var div = document.createElement('DIV');
			div.innerHTML = txt.replace(/<\/?[^>]+>/gi, '');
			return div.childNodes[0] ? div.childNodes[0].nodeValue : '';
		};
	}();
}

if (typeof Begun.Scripts != 'undefined') {
	Begun.Scripts.load("begun_utils.js");
}