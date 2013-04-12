Begun.Autocontext.Customization.setTpls({
	"begun_html_tpls": [
		{"block_160x600": '\
<div id="{{begun_alco_id}}" class="begun_adv begun_adv_fix begun_adv_fix_ver begun_adv_160x600"{{block_hover}}>\
<table class="begun_adv_sys"><tr>\
<td class="begun_adv_sys_logo" style="display:{{logo_display}}"><div><a href="{{begun_url}}" target="_blank" class="snap_noshots">begun</a></div></td>\
<td class="begun_adv_sys_sign_up" style="display:{{place_here_display}}"><div><a href="{{place_here_url}}" target="_blank" class="snap_noshots">{{place_here_text}}</a></div></td>\
</tr></table>\
<div class="begun_adv_common {{block_scroll_class}} banners_count_{{banners_count}} {{extended_block_class}}" id="{{scroll_div_id}}">\
<table class="begun_adv_table {{css_thumbnails}}" id="{{scroll_table_id}}">\
{{banners}}\
</table>\
</div>\
<div class="begun_adv_all" style="display:{{all_banners_display}}"><a href="{{all_banners_url}}" target="_blank" class="snap_noshots">{{all_banners_text}}</a></div>\
{{block_alco}}\
</div>\
'},
		{'banner_160x600': '\
<tr>\
<td class="begun_adv_cell" title="{{fullDomain}}" onclick="{{onclick}}" _url="{{url}}" _banner_id="{{banner_id}}">\
{{thumb}}\
<div class="begun_adv_block {{css_favicon}}" {{favicon}} title="{{fullDomain}}">\
<div class="begun_adv_title"><a class="snap_noshots" target="_blank" href="{{url}}" onmouseover="status=\'{{status}}\';return true" onmouseout="status=\'\';return true" title="{{fullDomain}}" {{favicon}}{{styleTitle}}>{{title}}</a>{{bnnr_alco}}</div>\
<div class="begun_adv_text"><a class="snap_noshots" target="_blank" href="{{url}}" onmouseover="status=\'{{status}}\';return true" onmouseout="status=\'\';return true" title="{{fullDomain}}"{{styleText}}>{{descr}}</a></div>\
<div class="begun_adv_contact"{{styleContact}}>{{contact}}</div>\
</div>\
</td>\
</tr>\
'},
		{'banner_160x600_rich': '\
<tr>\
<td class="begun_adv_cell begun_adv_rich" onclick="{{onclick}}" _url="{{url}}" _banner_id="{{banner_id}}">\
{{picture}}\
<div class="begun_adv_block {{css_favicon}}" {{favicon}} title="{{fullDomain}}">\
<div class="begun_adv_title"><a class="snap_noshots" target="_blank" href="{{url}}" onmouseover="status=\'{{status}}\';return true" onmouseout="status=\'\';return true" title="{{fullDomain}}"{{styleTitle}}>{{title}}</a></div>\
<div class="begun_adv_text"><a class="snap_noshots" target="_blank" href="{{url}}" onmouseover="status=\'{{status}}\';return true" onmouseout="status=\'\';return true" title="{{fullDomain}}"{{styleText}}>{{descr}}</a></div>\
<div class="begun_adv_contact"{{styleContact}}>{{contact}}</div>\
</div>\
</td>\
</tr>\
'}
	],
	"begun_css_tpls": [
		{"block_160x600": '\
#begun_block_{{block_id}} .begun_adv_160x600 .begun_scroll {\
	height: 560px !important;\
}\
#begun_block_{{block_id}} .begun_adv_160x600 .begun_adv_text,\
#begun_block_{{block_id}} .begun_adv_160x600 .begun_adv_text * {\
	font-size: 12px !important;\
	line-height: 13px !important;\
}\
#begun_block_{{block_id}} .begun_adv_160x600 .begun_adv_block {\
	width: 146px !important;\
	overflow: hidden !important;\
}\
#begun_block_{{block_id}} .begun_adv_160x600 {\
	width: 160px !important;\
	height: 600px !important;\
}\
#begun_block_{{block_id}} .begun_adv_160x600 .begun_adv_rich .begun_adv_image {\
	float:none !important;\
	top:0 !important;\
}\
#begun_block_{{block_id}} .begun_adv_160x600 .begun_adv_thumb .begun_thumb {\
	margin:5px 0 !important;\
	float:none !important;\
}\
#begun_block_{{block_id}} .begun_adv_160x600 .begun_adv_thumb .begun_thumb img {\
	float:none !important;\
}\
#begun_block_{{block_id}} .begun_adv_160x600 .begun_adv_thumb .begun_adv_block,\
#begun_block_{{block_id}} .begun_adv_160x600 .begun_adv_rich .begun_adv_block {\
	margin-left:0 !important;\
}\
#begun_block_{{block_id}} .begun_adv_160x600 .begun_adv_common {\
	height: 558px !important;\
}\
#begun_block_{{block_id}} .begun_adv_160x600 .begun_adv_common table {\
	height: 100% !important;\
}\
#begun_block_{{block_id}} .begun_adv_160x600 .begun_adv_common.banners_count_1,\
#begun_block_{{block_id}} .begun_adv_160x600 .begun_adv_common.banners_count_2 {\
	height: auto !important;\
}\
#begun_block_{{block_id}} .begun_adv_160x600 .begun_adv_common.banners_count_1 table,\
#begun_block_{{block_id}} .begun_adv_160x600 .begun_adv_common.banners_count_2 table {\
	height: auto !important;\
}\
#begun_block_{{block_id}} .begun_adv_160x600 .banners_count_3 .begun_adv_cell,\
#begun_block_{{block_id}} .begun_adv_160x600 .banners_count_3 .begun_adv_cell *,\
#begun_block_{{block_id}} .begun_adv_160x600 .banners_count_2 .begun_adv_cell,\
#begun_block_{{block_id}} .begun_adv_160x600 .banners_count_2 .begun_adv_cell * {\
	font-size:13px !important;\
	line-height:16px !important;\
}\
#begun_block_{{block_id}} .begun_adv_160x600 .banners_count_3 .begun_adv_phone,\
#begun_block_{{block_id}} .begun_adv_160x600 .banners_count_2 .begun_adv_phone {\
	margin-top:4px !important;\
}\
#begun_block_{{block_id}} .begun_adv_160x600 .banners_count_3 .begun_adv_text,\
#begun_block_{{block_id}} .begun_adv_160x600 .banners_count_3 .begun_adv_text *,\
#begun_block_{{block_id}} .begun_adv_160x600 .banners_count_2 .begun_adv_text,\
#begun_block_{{block_id}} .begun_adv_160x600 .banners_count_2 .begun_adv_text * {\
	font-size:14px !important;\
	line-height:18px !important;\
}\
#begun_block_{{block_id}} .begun_adv_160x600 .banners_count_3 .begun_adv_title,\
#begun_block_{{block_id}} .begun_adv_160x600 .banners_count_3 .begun_adv_title *,\
#begun_block_{{block_id}} .begun_adv_160x600 .banners_count_2 .begun_adv_title,\
#begun_block_{{block_id}} .begun_adv_160x600 .banners_count_2 .begun_adv_title * {\
	font-size:15px !important;\
	line-height:20px !important;\
}\
#begun_block_{{block_id}} .begun_adv_160x600 .banners_count_1 .begun_adv_cell,\
#begun_block_{{block_id}} .begun_adv_160x600 .banners_count_1 .begun_adv_cell * {\
	font-size:15px !important;\
	line-height:18px !important;\
	text-align:center !important;\
}\
#begun_block_{{block_id}} .begun_adv_160x600 .banners_count_1 .begun_adv_phone {\
	margin-top:5px !important;\
}\
#begun_block_{{block_id}} .begun_adv_160x600 .banners_count_1 .begun_adv_text,\
#begun_block_{{block_id}} .begun_adv_160x600 .banners_count_1 .begun_adv_text * {\
	font-size:16px !important;\
	line-height:20px !important;\
	text-align:center !important;\
}\
#begun_block_{{block_id}} .begun_adv_160x600 .banners_count_1 .begun_adv_title,\
#begun_block_{{block_id}} .begun_adv_160x600 .banners_count_1 .begun_adv_title * {\
	font-size:18px !important;\
	line-height:23px !important;\
	text-align:center !important;\
}\
#begun_block_{{block_id}} .begun_adv_160x600  .begun_adv_thumb,\
#begun_block_{{block_id}} .begun_adv_160x600 .begun_adv_thumb  *,\
#begun_block_{{block_id}} .begun_adv_160x600  .begun_adv_rich,\
#begun_block_{{block_id}} .begun_adv_160x600 .begun_adv_rich  * {\
	font-size:11px !important;\
	line-height:12px !important;\
}\
#begun_block_{{block_id}} .begun_adv_160x600  .begun_adv_thumb .begun_adv_text,\
#begun_block_{{block_id}} .begun_adv_160x600  .begun_adv_thumb .begun_adv_text *,\
#begun_block_{{block_id}} .begun_adv_160x600  .begun_adv_rich .begun_adv_text,\
#begun_block_{{block_id}} .begun_adv_160x600  .begun_adv_rich .begun_adv_text * {\
	font-size:12px !important;\
	line-height:13px !important;\
}\
#begun_block_{{block_id}} .begun_adv_160x600 .begun_adv_thumb .begun_adv_title,\
#begun_block_{{block_id}} .begun_adv_160x600 .begun_adv_thumb .begun_adv_title *,\
#begun_block_{{block_id}} .begun_adv_160x600 .begun_adv_rich .begun_adv_title,\
#begun_block_{{block_id}} .begun_adv_160x600 .begun_adv_rich .begun_adv_title * {\
	font-size:13px !important;\
	line-height:14px !important;\
}\
#begun_block_{{block_id}} .begun_adv_160x600  .banners_count_1 .begun_adv_thumb,\
#begun_block_{{block_id}} .begun_adv_160x600  .banners_count_1 .begun_adv_thumb  *,\
#begun_block_{{block_id}}.begun_auto_rich .begun_adv_160x600 .banners_count_1 .begun_adv_rich ,\
#begun_block_{{block_id}}.begun_auto_rich .begun_adv_160x600 .banners_count_1 .begun_adv_rich  * {\
	font-size:13px !important;\
	line-height:14px !important;\
}\
#begun_block_{{block_id}} .begun_adv_160x600  .banners_count_1 .begun_adv_thumb .begun_adv_text,\
#begun_block_{{block_id}} .begun_adv_160x600  .banners_count_1 .begun_adv_thumb .begun_adv_text *,\
#begun_block_{{block_id}}.begun_auto_rich .begun_adv_160x600 .banners_count_1 .begun_adv_rich .begun_adv_text,\
#begun_block_{{block_id}}.begun_auto_rich .begun_adv_160x600 .banners_count_1 .begun_adv_rich  .begun_adv_text * {\
	font-size:14px !important;\
	line-height:15px !important;\
}\
#begun_block_{{block_id}} .begun_adv_160x600 .banners_count_1 .begun_adv_thumb .begun_adv_title,\
#begun_block_{{block_id}} .begun_adv_160x600 .banners_count_1 .begun_adv_thumb .begun_adv_title *,\
#begun_block_{{block_id}}.begun_auto_rich .begun_adv_160x600 .banners_count_1 .begun_adv_title ,\
#begun_block_{{block_id}}.begun_auto_rich .begun_adv_160x600 .banners_count_1 .begun_adv_title  * {\
	font-size:15px !important;\
	line-height:16px !important;\
}\
#begun_block_{{block_id}} .begun_adv_160x600  .banners_count_2 .begun_adv_thumb,\
#begun_block_{{block_id}} .begun_adv_160x600  .banners_count_2 .begun_adv_thumb  *,\
#begun_block_{{block_id}}.begun_auto_rich .begun_adv_160x600 .banners_count_2 .begun_adv_rich ,\
#begun_block_{{block_id}}.begun_auto_rich .begun_adv_160x600 .banners_count_2 .begun_adv_rich  * {\
	font-size:12px !important;\
	line-height:13px !important;\
}\
#begun_block_{{block_id}} .begun_adv_160x600  .banners_count_2 .begun_adv_thumb .begun_adv_text,\
#begun_block_{{block_id}} .begun_adv_160x600  .banners_count_2 .begun_adv_thumb .begun_adv_text *,\
#begun_block_{{block_id}}.begun_auto_rich .begun_adv_160x600 .banners_count_2 .begun_adv_rich .begun_adv_text,\
#begun_block_{{block_id}}.begun_auto_rich .begun_adv_160x600 .banners_count_2 .begun_adv_rich  .begun_adv_text * {\
	font-size:13px !important;\
	line-height:14px !important;\
}\
#begun_block_{{block_id}} .begun_adv_160x600 .banners_count_2 .begun_adv_thumb .begun_adv_title,\
#begun_block_{{block_id}} .begun_adv_160x600 .banners_count_2 .begun_adv_thumb .begun_adv_title *,\
#begun_block_{{block_id}}.begun_auto_rich .begun_adv_160x600 .banners_count_2 .begun_adv_title ,\
#begun_block_{{block_id}}.begun_auto_rich .begun_adv_160x600 .banners_count_2 .begun_adv_title  * {\
	font-size:14px !important;\
	line-height:15px !important;\
}\
#begun_block_{{block_id}} .begun_adv_160x600 .begun_adv_common.banners_count_2,\
#begun_block_{{block_id}} .begun_adv_160x600 .begun_adv_common.banners_count_1 {\
	height:558px !important;\
}\
#begun_block_{{block_id}} .begun_adv_160x600 .begun_adv_common.banners_count_2 table,\
#begun_block_{{block_id}} .begun_adv_160x600 .begun_adv_common.banners_count_1 table {\
	height:100% !important;\
}\
#begun_block_{{block_id}} .begun_adv_160x600 .begun_adv_common.begun_extended_block {\
	height:100% !important;\
}\
#begun_block_{{block_id}} .begun_adv_160x600 .begun_adv_common.begun_extended_block_with_logo {\
	height:582px !important;\
}\
#begun_block_{{block_id}} #begun_alco_{{block_id}}.begun_adv_160x600 .begun_adv_common {\
	height:497px !important;\
}\
#begun_block_{{block_id}} .begun_adv_160x600 .begun_alco_message  {\
	padding:12px 9px 15px 17px !important;\
}\
#begun_block_{{block_id}} .begun_adv_160x600 .begun_alco_message span.begun_alco_attention {\
	left:8px !important;\
	_left:-8px !important;\
}\
\
\
#begun_block_{{block_id}} .begun_adv_accordion.begun_adv_240x400 .begun_adv_common {\
	position:relative !important;\
}\
#begun_block_{{block_id}} .begun_adv_240x400 {\
	width: 240px !important;\
	height: 400px !important;\
}\
#begun_block_{{block_id}} .begun_adv_240x400 .begun_scroll {\
	height: 358px !important;\
}\
#begun_block_{{block_id}} .begun_adv_240x400 .begun_adv_text,\
#begun_block_{{block_id}} .begun_adv_240x400 .begun_adv_text * {\
	font-size: 12px !important;\
	line-height: 13px !important;\
}\
#begun_block_{{block_id}} .begun_adv_240x400 .begun_adv_title,\
#begun_block_{{block_id}} .begun_adv_240x400 .begun_adv_title * {\
	font-size: 14px !important;\
	line-height: 14px !important;\
}\
#begun_block_{{block_id}} .begun_adv_240x400 .begun_adv_common {\
	position: relative !important;\
}\
#begun_block_{{block_id}} .begun_adv_240x400 .begun_adv_rich .begun_adv_block {\
	width:140px !important;\
}\
#begun_block_{{block_id}} .begun_adv_240x400 .begun_adv_common {\
	height: 361px !important;\
}\
#begun_block_{{block_id}} .begun_adv_240x400 .begun_adv_fav .begun_adv_title * {\
	font-size: 13px !important;\
	line-height: 13px !important;\
}\
#begun_block_{{block_id}} .begun_adv_240x400 .begun_adv_fav .begun_adv_text * {\
	line-height: 12px !important;\
}\
#begun_block_{{block_id}}  #begun_alco_{{block_id}}.begun_adv_240x400 .begun_adv_common {\
	height:321px !important;\
}\
#begun_block_{{block_id}} .begun_adv_240x400 .begun_adv_common table {\
	height: 100% !important;\
	width: 100% !important;\
}\
#begun_block_{{block_id}} .begun_adv_240x400 .begun_adv_common.banners_count_1 {\
	height: auto !important;\
}\
#begun_block_{{block_id}} .begun_adv_240x400 .begun_adv_common.banners_count_1 table {\
	height: auto !important;\
}\
#begun_block_{{block_id}} .begun_adv_240x400 .banners_count_1 .begun_adv_fav .begun_adv_title a {\
	padding-left:22px !important;\
	background-position:left 4px !important;\
	background-repeat:no-repeat !important;\
}\
#begun_block_{{block_id}} .begun_adv_240x400 .banners_count_3 .begun_adv_cell,\
#begun_block_{{block_id}} .begun_adv_240x400 .banners_count_3 .begun_adv_cell * {\
	font-size:13px !important;\
	line-height:15px !important;\
}\
#begun_block_{{block_id}} .begun_adv_240x400 .banners_count_3 .begun_adv_phone {\
	margin-top:3px !important;\
}\
#begun_block_{{block_id}} .begun_adv_240x400 .banners_count_3 .begun_adv_text,\
#begun_block_{{block_id}} .begun_adv_240x400 .banners_count_3 .begun_adv_text * {\
	font-size:14px !important;\
	line-height:17px !important;\
}\
#begun_block_{{block_id}} .begun_adv_240x400 .banners_count_3 .begun_adv_title,\
#begun_block_{{block_id}} .begun_adv_240x400 .banners_count_3 .begun_adv_title * {\
	font-size:15px !important;\
	line-height:17px !important;\
}\
#begun_block_{{block_id}}  #begun_alco_{{block_id}}.begun_adv_240x400 .banners_count_3 .begun_adv_cell,\
#begun_block_{{block_id}}  #begun_alco_{{block_id}}.begun_adv_240x400 .banners_count_3 .begun_adv_cell * {\
	font-size:12px !important;\
	line-height:14px !important;\
}\
#begun_block_{{block_id}}  #begun_alco_{{block_id}}.begun_adv_240x400 .banners_count_3 .begun_adv_phone {\
	margin-top:2px !important;\
}\
#begun_block_{{block_id}}  #begun_alco_{{block_id}}.begun_adv_240x400 .banners_count_3 .begun_adv_text,\
#begun_block_{{block_id}}  #begun_alco_{{block_id}}.begun_adv_240x400 .banners_count_3 .begun_adv_text * {\
	font-size:13px !important;\
	line-height:16px !important;\
}\
#begun_block_{{block_id}}  #begun_alco_{{block_id}}.begun_adv_240x400 .banners_count_3 .begun_adv_title,\
#begun_block_{{block_id}}  #begun_alco_{{block_id}}.begun_adv_240x400 .banners_count_3 .begun_adv_title * {\
	font-size:14px !important;\
	line-height:13px !important;\
}\
#begun_block_{{block_id}} .begun_adv_240x400 .banners_count_2 .begun_adv_cell,\
#begun_block_{{block_id}} .begun_adv_240x400 .banners_count_2 .begun_adv_cell * {\
	font-size:13px !important;\
	line-height:17px !important;\
}\
#begun_block_{{block_id}} .begun_adv_240x400 .banners_count_2 .begun_adv_phone {\
	margin-top:4px !important;\
}\
#begun_block_{{block_id}} .begun_adv_240x400 .banners_count_2 .begun_adv_text,\
#begun_block_{{block_id}} .begun_adv_240x400 .banners_count_2 .begun_adv_text * {\
	font-size:14px !important;\
	line-height:19px !important;\
}\
#begun_block_{{block_id}} .begun_adv_240x400 .banners_count_2 .begun_adv_title,\
#begun_block_{{block_id}} .begun_adv_240x400 .banners_count_2 .begun_adv_title * {\
	font-size:16px !important;\
	line-height:18px !important;\
}\
#begun_block_{{block_id}} .begun_adv_240x400 .banners_count_1 .begun_adv_cell,\
#begun_block_{{block_id}} .begun_adv_240x400 .banners_count_1 .begun_adv_cell * {\
	font-size:15px !important;\
	line-height:19px !important;\
	text-align:center !important;\
}\
#begun_block_{{block_id}} .begun_adv_240x400 .banners_count_1 .begun_adv_phone {\
	margin-top:5px !important;\
}\
#begun_block_{{block_id}} .begun_adv_240x400 .banners_count_1 .begun_adv_text,\
#begun_block_{{block_id}} .begun_adv_240x400 .banners_count_1 .begun_adv_text * {\
	font-size:16px !important;\
	line-height:19px !important;\
	text-align:center !important;\
}\
#begun_block_{{block_id}} .begun_adv_240x400 .banners_count_1 .begun_adv_title,\
#begun_block_{{block_id}} .begun_adv_240x400 .banners_count_1 .begun_adv_title * {\
	font-size:18px !important;\
	line-height:25px !important;\
	text-align:center !important;\
}\
#begun_block_{{block_id}} .begun_adv_240x400 .begun_adv_common.banners_count_1 {\
	height:361px !important;\
}\
#begun_block_{{block_id}} .begun_adv_240x400 .begun_adv_common.banners_count_1 table {\
	height:100% !important;\
}\
#begun_block_{{block_id}}.begun_auto_rich .begun_adv_240x400 .banners_count_3 .begun_adv_contact,\
#begun_block_{{block_id}}.begun_auto_rich .begun_adv_240x400 .banners_count_3 .begun_adv_contact *,\
#begun_block_{{block_id}}.begun_auto_rich .begun_adv_240x400 .banners_count_3 .begun_adv_cell,\
#begun_block_{{block_id}}.begun_auto_rich .begun_adv_240x400 .banners_count_3 .begun_adv_cell * {\
	font-size: 11px !important;\
	line-height: 11px !important;\
}\
#begun_block_{{block_id}}.begun_auto_rich .begun_adv_240x400 .banners_count_3 .begun_adv_text,\
#begun_block_{{block_id}}.begun_auto_rich .begun_adv_240x400 .banners_count_3 .begun_adv_text * {\
	font-size: 12px !important;\
	line-height: 12px !important;\
}\
#begun_block_{{block_id}}.begun_auto_rich .begun_adv_240x400 .banners_count_3  .begun_adv_title,\
#begun_block_{{block_id}}.begun_auto_rich .begun_adv_240x400 .banners_count_3  .begun_adv_title * {\
	font-size: 13px !important;\
	line-height: 13px !important;\
}\
#begun_block_{{block_id}}.begun_auto_rich .begun_adv_240x400 .banners_count_2 .begun_adv_contact,\
#begun_block_{{block_id}}.begun_auto_rich .begun_adv_240x400 .banners_count_2 .begun_adv_contact *,\
#begun_block_{{block_id}}.begun_auto_rich .begun_adv_240x400 .banners_count_2 .begun_adv_cell,\
#begun_block_{{block_id}}.begun_auto_rich .begun_adv_240x400 .banners_count_2 .begun_adv_cell * {\
	font-size: 12px !important;\
	line-height: 13px !important;\
}\
#begun_block_{{block_id}}.begun_auto_rich .begun_adv_240x400 .banners_count_2 .begun_adv_text,\
#begun_block_{{block_id}}.begun_auto_rich .begun_adv_240x400 .banners_count_2 .begun_adv_text * {\
	font-size: 13px !important;\
	line-height: 14px !important;\
}\
#begun_block_{{block_id}}.begun_auto_rich .begun_adv_240x400 .banners_count_2  .begun_adv_title,\
#begun_block_{{block_id}}.begun_auto_rich .begun_adv_240x400 .banners_count_2  .begun_adv_title * {\
	font-size: 14px !important;\
	line-height: 15px !important;\
}\
#begun_block_{{block_id}}.begun_auto_rich .begun_adv_240x400 .banners_count_1 .begun_adv_contact,\
#begun_block_{{block_id}}.begun_auto_rich .begun_adv_240x400 .banners_count_1 .begun_adv_contact *,\
#begun_block_{{block_id}}.begun_auto_rich .begun_adv_240x400 .banners_count_1 .begun_adv_cell,\
#begun_block_{{block_id}}.begun_auto_rich .begun_adv_240x400 .banners_count_1 .begun_adv_cell * {\
	font-size: 13px !important;\
	line-height: 14px !important;\
}\
#begun_block_{{block_id}}.begun_auto_rich .begun_adv_240x400 .banners_count_1 .begun_adv_text,\
#begun_block_{{block_id}}.begun_auto_rich .begun_adv_240x400 .banners_count_1 .begun_adv_text * {\
	font-size: 14px !important;\
	line-height: 15px !important;\
}\
#begun_block_{{block_id}}.begun_auto_rich .begun_adv_240x400 .banners_count_1 .begun_adv_title,\
#begun_block_{{block_id}}.begun_auto_rich .begun_adv_240x400 .banners_count_1  .begun_adv_title * {\
	font-size: 16px !important;\
	line-height: 17px !important;\
}\
#begun_block_{{block_id}} .begun_adv_240x400 .begun_adv_common.begun_extended_block {\
	height:100% !important;\
}\
#begun_block_{{block_id}} .begun_adv_240x400 .begun_adv_common.begun_extended_block_with_logo {\
	height:382px !important;\
}\
#begun_block_{{block_id}} .begun_adv_240x400 .begun_alco_message {\
	padding:10px 10px 12px 20px !important;\
}\
#begun_block_{{block_id}} .begun_adv_240x400 .begun_alco_message span.begun_alco_attention {\
	_left:-10px !important;\
}\
'}
	]
});
/*$LastChangedRevision: 36546 $*/
Begun.Autocontext.tplLoaded("begun_tpl_block_160x600");
