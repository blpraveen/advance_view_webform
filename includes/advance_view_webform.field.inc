<?php
/**
 */
/*



/**
 * Implements hook_field_info().
 */
function advance_view_webform_field_info() {
  return array(
    'webform_group' => array(
      'label' => t('Group'),
      'description' => t('This field show dropdown groups.'),
      'default_widget' => 'webform_group_widget',
      'property_type' => 'field_item_webform_group',
      //'default_formatter' => 'webform_group_default',
      //'property_callbacks' => array(
        //'webform_group_metadata_field_property_callback',
      //),
    ),
  );
}




/**
 * Implements hook_field_widget_info().
 */
function advance_view_webform_field_widget_info() {
  return array(
    'webform_group_widget' => array(
      'label' => t('Webform Group Select'),
      'field types' => array('webform_group'),
      'behaviors' => array(
        'multiple values' => FIELD_BEHAVIOR_CUSTOM,
        'default value' => FIELD_BEHAVIOR_NONE,
      ),
    ),
  );
}



/**
 * Implements hook_field_widget_settings_form().
 */
function webform_group_field_widget_settings_form($field, $instance) {

  $form = array();
  return $form;	
}

/**
 * Implements hook_field_widget_form().
 */
function advance_view_webform_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  $type  = $instance['widget']['type'];
  // Build an array of existing term IDs.

  switch($type) {
	case 'webform_group_widget': 
		$element += array(
		    '#type'          => 'webform_tree_dropdown',
		    '#config'        => array(
		      'module' =>  $delta,
		      'params' => array(
			'vid'                        => NULL,
			'root_term'                  => NULL,
			'entity_count_for_node_type' => NULL,
		      ),
		    ),
		    '#default_value' => isset($items) ? $items[0]['input_select'] : 0,
	      );
  }

  $element_info = element_info($delta);
  //$element['#process'] = array_merge($element_info['#process'], array('webform_group_widget_process'));
  $elements = array($element);
  return $elements;
}

/**
 * #process callback that runs after HS' #process callback, to transform
 * #return_value to the format that Field API/Taxonomy Field expects.
 */
function webform_group_widget_process($element, &$form_state, $complete_form) {


}


/**
 * Implements hook_field_widget_error().
 */
function webform_group_taxonomy_field_widget_error($element, $error, $form, &$form_state) {
  form_error($element, $error['message']);
}



/**
 * Implementation of hook_hierarchical_select_root_level().
 */
function advance_view_webform_hierarchical_select_root_level($params) {
  if (!isset($params['vid'])) {
    return array();
  }
  // TODO: support multiple parents, i.e. support "save lineage".
  $vocabulary = taxonomy_vocabulary_load($params['vid']);
  $terms = advance_view_webform_hierarchical_select_get_tree($params['vid'], 0, -1, 1);

  // If the root_term parameter is enabled, then prepend a fake "<root>" term.
  if (isset($params['root_term']) && $params['root_term'] === TRUE) {
    $root_term = new StdClass();
    $root_term->tid = 0;
    $root_term->name = '<' . t('root') . '>';
    $terms = array_merge(array($root_term), $terms);
  }

  // Unset the term that's being excluded, if it is among the terms.
  if (isset($params['exclude_tid'])) {
    foreach ($terms as $key => $term) {
      if ($term->tid == $params['exclude_tid']) {
        unset($terms[$key]);
      }
    }
  }

  // If the Term Permissions module is installed, honor its settings.
  if (function_exists('term_permissions_allowed')) {
    global $user;
    foreach ($terms as $key => $term) {
      if (!term_permissions_allowed($term->tid, $user) ) {
        unset($terms[$key]);
      }
    }
  }

  return advance_view_webform_hierarchical_select_terms_to_options($terms);
}





/**
 * Transform an array of terms into an associative array of options, for use
 * in a select form item.
 *
 * @param $terms
 *  An array of term objects.
 * @return
 *  An associative array of options, keys are tids, values are term names.
 */
function advance_view_webform_hierarchical_select_terms_to_options($terms) {
  $options = array();
  $use_i18n = module_exists('i18n_taxonomy');
  foreach ($terms as $key => $term) {
    // Use the translated term when available!
    $options[$term->tid] = $use_i18n && isset($term->vid) ? i18n_taxonomy_term_name($term) : $term->name;
  }
  return $options;
}



//----------------------------------------------------------------------------
// Private functions.

/**
 * Drupal core's taxonomy_get_tree() doesn't allow us to reset the cached
 * trees, which obviously causes problems when you create new items between
 * two calls to it.
 */
function advance_view_webform_hierarchical_select_get_tree($vid, $parent = 0, $depth = -1, $max_depth = NULL, $reset = FALSE) {
  static $children, $parents, $terms;

  if ($reset) {
    $children = $parents = $terms = array();
  }

  $depth++;

  // We cache trees, so it's not CPU-intensive to call get_tree() on a term
  // and its children, too.
  if (!isset($children[$vid])) {
    $children[$vid] = array();

    $query = db_select('taxonomy_term_data', 't');
    $query->join('taxonomy_term_hierarchy', 'h', 'h.tid = t.tid');
    $result = $query
      ->addTag('translatable')
      ->addTag('term_access')
      ->addTag('hs_taxonomy_tree')
      ->fields('t')
      ->fields('h', array('parent'))
      ->condition('t.vid', $vid)
      ->orderBy('t.weight')
      ->orderBy('t.name')
      ->execute();

    foreach ($result as $term) {
      $children[$vid][$term->parent][] = $term->tid;
      $parents[$vid][$term->tid][] = $term->parent;
      $terms[$vid][$term->tid] = $term;
    }
  }

  // Provide support for Title module. If Title module is enabled and this
  // vocabulary uses translated term names we want output those terms with their
  // translated version. Therefore a full taxonomy term entity load is required,
  // similar to taxonomy_get_tree().
  if (module_exists('title')) {
    $vocabulary = taxonomy_vocabulary_load($vid);
    if (title_field_replacement_enabled('taxonomy_term', $vocabulary->machine_name, 'name')) {
      $term_entities = taxonomy_term_load_multiple(array_keys($terms[$vid]));
    }
  }

  $max_depth = (is_null($max_depth)) ? count($children[$vid]) : $max_depth;
  if (isset($children[$vid][$parent])) {
    foreach ($children[$vid][$parent] as $child) {
      if ($max_depth > $depth) {
        $term = isset($term_entities[$child]) ? $term_entities[$child] : $terms[$vid][$child];
        $term = clone $term;
        $term->depth = $depth;
        // The "parent" attribute is not useful, as it would show one parent only.
        unset($term->parent);
        $term->parents = $parents[$vid][$child];
        $tree[] = $term;

        if (isset($children[$vid][$child])) {
          $tree = array_merge($tree, advance_view_webform_hierarchical_select_get_tree($vid, $child, $depth, $max_depth));
        }
      }
    }
  }

  return isset($tree) ? $tree : array();
}




/**
 * Implements hook_element_info().
 */
function advance_view_webform_element_info() {
  $types['webform_tree_dropdown'] = array(
    '#input' => TRUE,
    '#process' => array('form_webforms_group_dropdown_process'),
    '#theme' => array('webform_tree_dropdown_select'),
    '#theme_wrappers' => array('form_element'),
    '#value_callback' => 'webform_tree_dropdown_value_callback',
    '#config' => array(
      'module' => 'some_module',
      'params' => array(),
      'input_select' => array(),
    ),
    '#default_value' => 0,
  );

  return $types;
}




/**
 * select form element type #process callback.
 */
function form_webforms_group_dropdown_process($element, &$form_state, $complete_form) {
  if (arg(0) != 'webfrom_treedropdown_ajax') {
    // Get unique identifier using parents of the field.
    $cid = isset($element['#parents']) ? implode("-", $element['#parents']) : implode("-", $element['#field_parents']);

    // Verify if hsid is present.
    $elwftdid = drupal_array_get_nested_value($element, array('#value', 'wftdid'));

    if (!isset($elwftdid)) {
      // Retrieve previous element from form_state.
      $cached = drupal_array_get_nested_value($form_state, array('storage', 'wftd', 'wftd_fields', $cid));
    }
    if (empty($cached)) {
      $docache = TRUE;
    }
    else {
      // Switch current element with the "cached".
      return $cached;
    }
  }

  // Determine the HSID.
  $wftdid = _webform_tree_dropdown_process_determine_wftdid($element, $form_state);
  // Config.
  $config = $element['#config'];

  // Attach CSS/JS files and JS settings.
  $element = _webform_tree_dropdown_process_attach_css_js($element, $wftdid, $form_state, $complete_form);

  // Developer mode diagnostics, return immediately in case of a config error.
  /*if (!_hs_process_developer_mode_log_diagnostics($element)) {
    return $element;
  }*/
 // Render the hierarchical select.
  $element['webform_tree_dropdown'] = array(
    '#theme' => 'webform_tree_dropdown_select_selects_container',
  );

  list($element, $form_state) =  _webforms_tree_dropdown_process_render_input($element,$wftdid, $form_state);

  // Ensure the render order is correct.
  $element['webform_tree_dropdown']['#weight']   = 0;

  // Calculate the selections in both the hierarchical select and the dropbox,
  // we need these before we can render anything.
  //$hs_selection = $db_selection = array();
  //list($hs_selection, $db_selection) = _hierarchical_select_process_calculate_selections($element, $hsid, $form_state);


  // Generate the $hierarchy and $dropbox objects using the selections that
  // were just calculated.
  //$dropbox = (!$config['dropbox']['status']) ? FALSE : _hierarchical_select_dropbox_generate($config, $db_selection);
  //$hierarchy = _hierarchical_select_hierarchy_generate($config, $hs_selection, $element['#required'], $dropbox);
 
  $element['#return_value'] = isset($element['#value']['webform_tree_dropdown']['input_select']) ?  $element['#value']['webform_tree_dropdown']['input_select'] : $element['#default_value'];
  // Add a validate callback, which will:
  // - validate that the dropbox limit was not exceeded.
  // - set the return value of this form element.
  // Also make sure it is the *first* validate callback.
  $element['#element_validate'] = (isset($element['#element_validate'])) ? $element['#element_validate'] : array();
  $element['#element_validate'] = array_merge(array('_webform_tree_dropdown_validate'), $element['#element_validate']);

  // Ensure the form is cached, for AJAX to work.
  $form_state['cache'] = FALSE;

  //
  // Rendering.
  //

  // Ensure that #tree is enabled!
  $element['#tree'] = TRUE;

  // Store the HSID in a hidden form element; when an AJAX callback comes in,
  // we'll know which wftid was updated.
  $element['wftdid'] = array('#type' => 'hidden', '#value' => $wftdid);

  $group_name = '';
  if($element['#return_value'] > 0 ) {
   	$group = advance_view_module_get_group($element['#return_value']);	
	$group_name = $group->name;
  }
  $element['webform_tree_dropdown']['group_name'] =  array('#type' => 'text', '#value' => $group_name);
  // Render the hierarchical select.
  /*$element['webform_tree_dropdown'] = array(
    '#theme' => 'hierarchical_select_selects_container',
  );*/
  $size = isset($element['#size']) ? $element['#size'] : 0;
  //$element['webform_tree_dropdown']['selects'] = _hs_process_render_hs_selects($hsid, $hierarchy, $size);



  return $element;
}

/**
 * Hierarchical select form element #element_validate callback.
 */
function _webform_tree_dropdown_validate(&$element, &$form_state) {

  $wftdid = $element['wftdid']['#value'];


  // Set the proper return value. I.e. instead of returning all the values
  // that are used for making the hierarchical_select form element type work,
  // we pass a flat array of item ids. e.g. for the taxonomy module, this will
  // be an array of term ids. If a single item is selected, this will not be
  // an array.
  // If the form item is disabled, set the default value as the return value,
  // because otherwise nothing would be returned (disabled form items are not
  // submitted, as described in the HTML standard).
  if (isset($element['#disabled']) && $element['#disabled']) {
    $element['#return_value'] = $element['#default_value'];
  }
  /*echo '<pre>';
  print_r($form_state['node']);
  exit();*/
 
  $element['#value'] = $element['#return_value'];
  form_set_value($element, $element['#value'], $form_state);

  // We have to check again for errors. This line is taken litterally from
  // form.inc, so it works in an identical way.
  if ($element['#required'] &&
    (!count($element['#value']) || (is_string($element['#value']) && strlen(trim($element['#value'])) == 0))) {
    form_error($element, t('!name field is required.', array('!name' => $element['#title'])));
    _webform_tree_dropdown_form_set_error_class($element);
  }
}


/**
 * Set the 'error' class on the appropriate part 
 * depending on its configuration.
 *
 * @param $element
 *  
 */
function _webform_tree_dropdown_form_set_error_class(&$element) {
	form_error($element['webform_tree_dropdown']['input_select']);
}

function _webform_tree_dropdown_process_determine_wftdid($element, &$form_state) {

  if (!isset($element['#value']) || !is_array($element['#value']) || !array_key_exists('wftdid', $element['#value'])) {
    $wftdid = uniqid();
  }
  else {
    $wftdid = check_plain($element['#value']['wftdid']);
  }

  return $wftdid;
}



function _webform_tree_dropdown_process_attach_css_js($element, $wftdid, &$form_state, $complete_form) {
  global $language;
  // Set up Javascript and add settings specifically for the current
  // hierarchical select.
  $element['#attached']['js'] = array( libraries_get_path('jstree') . '/jquery.jstree.js',	
				drupal_get_path('module', 'advance_view_webform') . '/js/treeDropdown.js');
  //drupal_get_path('module', 'advance_view_webform') . '/js/jstree.settings.js'				
  $element['#attached']['css'] = array( libraries_get_path('jstree') . '/themes/apple/style.css', drupal_get_path('module', 'advance_view_webform') . '/css/dropdown.css');
  if (!isset($form_state['storage']['hs']['js_settings_sent'])) {
    $form_state['storage']['hs']['js_settings_sent'] = array();
  }

  // Form was submitted; this is a newly loaded page, thus ensure that all JS
  // settings are resent.
  if ($form_state['process_input'] === TRUE) {
    $form_state['storage']['wftd']['js_settings_sent'] = array();
  }

  if (!isset($form_state['storage']['wftd']['js_settings_sent'][$wftdid]) || (isset($form_state['storage']['wftd']['js_settings_sent'][$wftdid]) && (isset($form_state['triggering_element']) && $form_state['triggering_element']['#type'] == 'submit'))) {
    $config = _webform_tree_dropdown_inherit_default_config($element['#config']);
    $settings =  array(
      'WebformTreeDropDown' => array(
        // Save language in settings so we can use the same language during the AJAX callback.
        'wftd_current_language' => $language->language,
        'settings' => array(
          "wftd-$wftdid" => array(
            'ajax_url'         => url('webforms_group_tree_ajax/' . implode('/', $element['#array_parents'])),
          ),
        ),            
      )
    );

    if (!isset($_POST['wftdid'])) {
      $element['#attached']['js'][] = array(
        'type' => 'setting',
        'data' => $settings,
      );
    }
    else {
      $element['#attached']['_wftd_new_setting_ajax'][] = array($hsid, $settings['WebformTreeDropDown']['settings']["wftd-$wftdid"]);
    }

    $form_state['storage']['wftd']['js_settings_sent'][$wftdid] = TRUE;
  }

  return $element;
}


function _webform_tree_dropdown_inherit_default_config($config, $defaults_override = array()) {

  $type = advance_view_webform_element_info();
  $defaults = $type['webform_tree_dropdown']['#config'];
  // Don't inherit the module and params settings.
  unset($defaults['module']);
  unset($defaults['params']);

  // Allow the defaults to be overridden.
  $defaults = array_smart_merge($defaults, $defaults_override);

  // Apply the defaults to the config.
  $config = array_smart_merge($defaults, $config);

  return $config;
}

function _webforms_tree_dropdown_process_render_input($element,$wftdid,$form_state) {

      $element['webform_tree_dropdown']['input_select'] = array(
        '#type'                    => 'hidden',
        '#value'                   => '',
        '#attributes'              => array('class' => array('hidden-dropdown-field')),
        '#validate'                => array(),
      );

     return array($element, $form_state);
}


function webforms_group_tree_ajax() {
	$groups = advance_view_module_sub_groups(0);
	$output = '';	
	if(count($groups)) {
		foreach($groups as $group) {
			if(check_group_as_child($group->gid)) {
				$group->has_child  = true;
				$group->child_output =  advance_view_module_sub_groups_list($group);
			} else {
				$group->has_child  = false;	
				$group->child_output = '';
			}

			
		}
		$output .= theme('webforms_group_tree_list',array('items' => $groups));				
	}

	$commands[] = array(
	'command' => 'webformGroupUpdate',
	'output'  => $output,
	);

        return array('#type' => 'ajax', '#commands' => $commands);

}

function advance_view_module_sub_groups_list($grp) {
	$output = '';
	if (is_object($grp)) {
		$groups = advance_view_module_sub_groups($grp->gid);	
		if(count($groups)) {
			foreach($groups as $group) {
				if(check_group_as_child($group->gid)) {
					$group->has_child  = true;
					$group->child_output =  advance_view_module_sub_groups_list($group);
				} else {
					$group->has_child  = false;	
					$group->child_output = '';
				}

			}
			$output .= theme('webforms_group_tree_list',array('items' => $groups));				
		}
	}
	return $output;
}

function webform_tree_dropdown_value_callback($element, $input = FALSE, &$form_state) {
	if ($input !== FALSE) {
		if ($input['webform_tree_dropdown']['input_select']) {
			$input['value'] = $input['webform_tree_dropdown']['input_select'];
		}else {
			$input['webform_tree_dropdown']['input_select']	= $element['#default_value'];
			$input['value'] = $element['#default_value'];	
		}

		//$form_state['node']->{$element['#field_name']}[$element['#language']]= array($input);
	    return $input;
	}  elseif (!empty($element['#default_value'])) {
	   return $element['#default_value'];
  	}
	return;
}


function advance_view_webform_field_is_empty($item, $field){
	  /*if (!isset($item) ||  $item == '') {
	      return FALSE;
	  }*/
	 return FALSE;
}

function advance_view_webform_field_formatter_info() {
	return array(
		'webform_group' => array(
			'label' => t('Default'),
			'field types' => array('webform_group'),
		),
	);
} 

/*
function advance_view_webform_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
	$element = array();
		switch ($display['type']) {
			case 'webform_group_default' :
				foreach ($items as $delta => $item) {
					if (isset($item['input_select'])) {
						$element[$delta]['#markup'] = $item['input_select'];
					}
				}
			break;
		}
	return $element;
} */


