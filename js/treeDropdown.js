;(function ( $, window, document, undefined ) {
	/*$.fn.getTreeConfig = function(){
		var cfg = this._super();
		cfg.checkbox = {override_ui: true, two_state: true};
		cfg.plugins.push('checkbox');
		cfg.ui.select_limit = -1;
		return cfg;   
	};*/
	var 
		pluginName = "treeDropDown",
		defaults = {
			ajax_url : '?q=webforms_group_tree_ajax',
			onOpen: function() {},
			onClose: function() {},
		};
	
	function TreeDropDown (element, options) {
		this.element 	= element;
		this.obj 	= $(this.element);
		this.treewrap 	= this.obj.find('.webform-treedropdown');
		this.title 	= this.obj.find('.treedropdownfield-title');
		this.hidden 	= this.obj.find('.hidden-dropdown-field');


		this.options 	= $.extend( {}, defaults, options );
		this.defaults 	= defaults;
		this.name 	= pluginName;
		this.RequestParams = null;
		this.CurrentXhr = null;
		init.apply(this);

	}

	/******************* 
		Private methods 
	  		There is simply no need to expose these
	*/
	var init = function() {
		this.panel 	= this.obj.find('.treedropdownfield-panel');
		this.applyEvents();
	}
	
	TreeDropDown.prototype.applyEvents = function()
	{	
		var $this = this
		this.obj.click(function(e) {
			e.preventDefault();
			console.log('asds');
			$this.togglePanel();
		});
		this.obj.on('click', '.tree-holder .has-child .jstree-clicked,.tree-holder .no-child .jstree-clicked',function(e) {
			e.preventDefault();
			$this.hidden.val($(this).attr('data-gid'));
			$this.title.html($(this).attr('data-name'));
			$this.closePanel();
		});
		this.obj.on('click', '.tree-holder' , function(e) {
			e.stopPropagation();
		});


	}
	TreeDropDown.prototype.togglePanel = function()
	{	

		this[this.panel.is(':visible') ? 'closePanel' : 'openPanel']();

	}
	TreeDropDown.prototype.loadTree =  function() {
		
	}
	TreeDropDown.prototype.openPanel =  function() {

		
		var panel = this.panel, tree = this.panel.find('.tree-holder');

		panel.css('width', this.treewrap.width());
		
		panel.show();
		
		// swap the down arrow with an up arrow
		var toggle = this.obj.find(".treedropdownfield-toggle-panel-link");
		toggle.addClass('treedropdownfield-open-tree');
		this.obj.addClass("treedropdownfield-open-tree");
		
		toggle.find("a")
			.removeClass('ui-icon-triangle-1-s')
			.addClass('ui-icon-triangle-1-n');
		
		if(tree.is(':empty') && !panel.hasClass('loading')) {
			this.loadTree(null, this.riseUp);
		} else {
			this.riseUp();
		}

		this.obj.trigger('panelshow');
	}
	TreeDropDown.prototype.riseUp =  function() {
		var container = this.obj,
			dropdown = this.panel,
			toggle = this.obj.find(".treedropdownfield-toggle-panel-link"),
			offsetTop = toggle.innerHeight(),
			elHeight,
			elPos,
			endOfWindow;

		if (toggle.length > 0) {
			endOfWindow = ($(window).height() + $(document).scrollTop()) - toggle.innerHeight();
			elPos = toggle.offset().top;
			elHeight = dropdown.innerHeight();
			
			// If the dropdown is too close to the bottom of the page, position it above the 'trigger'
			if (elPos + elHeight > endOfWindow && elPos - elHeight > 0) {
				container.addClass('treedropdownfield-with-rise');
				offsetTop = -dropdown.outerHeight();
			} else {
				container.removeClass('treedropdownfield-with-rise');
			}
		}
		dropdown.css({"top": offsetTop + "px"});
	}
	TreeDropDown.prototype.closePanel =  function() {

		// swap the up arrow with a down arrow
		var toggle = this.obj.find(".treedropdownfield-toggle-panel-link");
		toggle.removeClass('treedropdownfield-open-tree');
		this.obj.removeClass('treedropdownfield-open-tree treedropdownfield-with-rise');
						
		toggle.find("a")
			.removeClass('ui-icon-triangle-1-n')
			.addClass('ui-icon-triangle-1-s');
		
		this.panel.hide();
		this.obj.trigger('panelhide');
	}

	TreeDropDown.prototype.loadTree =  function(params, callback) {
		var self = this, panel = this.panel, treeHolder = $(panel).find('.tree-holder'),
			params = (params) ? $.extend({}, this.RequestParams, params) : this.RequestParams, xhr;

		if(this.CurrentXhr) this.CurrentXhr.abort();
		panel.addClass('loading');
		xhr = $.ajax({
			url: this.options.ajax_url,
			data: params,
			dataType:'json',
			complete: function(xhr, status) {
				panel.removeClass('loading');
			},
			success: function(data, status, xhr) {
				treeHolder.html(data[1].output);
				var firstLoad = true;
				treeHolder.jstree();
				/*treeHolder
					.jstree('destroy')
					.bind('loaded.jstree', function(e, data) {
						var val = self.getValue(), selectNode = treeHolder.find('*[data-id="' + val + '"]'), 
							currentNode = data.inst.get_selected();
						if(val && selectNode != currentNode) data.inst.select_node(selectNode);
						firstLoad = false;
						if(callback) callback.apply(self);
					})
					.jstree(self.options.getTreeConfig())
					.bind('select_node.jstree', function(e, data) {
						var node = data.rslt.obj, id = $(node).data('id');
						if(!firstLoad && self.getValue() == id) {
							// Value is already selected, unselect it (for lack of a better UI to do this)
							self.data('metadata', null);
							self.setTitle(null);
							self.setValue(null);
							data.inst.deselect_node(node);
						} else {
							self.data('metadata', $.extend({id: id}, $(node).getMetaData()));
							self.setTitle(data.inst.get_text(node));
							self.setValue(id);
						}
						
						// Avoid auto-closing panel on first load
						if(!firstLoad) self.closePanel();
						firstLoad=false;
					});*/

				self.CurrentXhr = null;
			}
		});
		this.CurrentXhr = xhr;
	}
	/**************
		Constructor
	*/	
	$.fn[pluginName] = function (options) {
		return $(this).each(function() {
			if (!$.data(this, pluginName)) {
				$.data(this, pluginName, new TreeDropDown(this, options));
			}
		});
	};

	jQuery(document).ready(function() {

		$('div.drupal-jstree').each(function () {
			 $(this).treeDropDown();
		});
	});

})(jQuery, window, document);
