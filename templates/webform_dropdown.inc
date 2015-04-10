<?php

/**
 * @file
 * All theme functions for the Webform Dropdown module.
 */



/**
 * Format a webform dropdown.
 *
 * @param $element
 *   An associative array containing the properties of the element.
 * @return
 *   A themed HTML string representing the form element.
 */
function theme_webform_tree_dropdown_select($variables) {
  $element = $variables['element'];
  $output = '';
  // Update $element['#attributes']['class'].
  if (!isset($element['#attributes']['class'])) {
    $element['#attributes']['class'] = array();
  }
  $wftdid = $element['wftdid']['#value'];


  $classes = array(
   'drupal-jstree',
   "webform-tree-dropdown-jstree",
   // Classes that make it possible to override the styling of specific
   // instances of Hierarchical Select, based on either the ID of the form
   // element or the config that it uses.
   'webform-tree-dropdown-' . $element['#id'],
  );
  $element['#attributes']['class'] = array_merge($element['#attributes']['class'], $classes);
  $element['#attributes']['id'] = "webform-tree-dropdown-$wftdid-wrapper";
  $element['#id'] = "webform-tree-dropdown-$wftdid-wrapper"; // This ensures the label's for attribute is correct.	
  return '<div ' . drupal_attributes($element['#attributes']) . '>' . drupal_render_children($element) . '</div>';
}



function theme_webform_tree_dropdown_select_selects_container($variables) {
  $element = $variables['element'];
  $group_name = $element['group_name']['#value'];
  unset($element['group_name']);
  $output = '';
  $output .= '<div class="webform-treedropdown clearfix">';
  $output .= drupal_render_children($element);
  $output .= '<span class="treedropdownfield-title">'. $group_name .'</span>';
  $output .= '<div class="treedropdownfield-toggle-panel-link"><a href="#" class="ui-icon ui-icon-triangle-1-s"></a></div>';
  $output .= '<div class="treedropdownfield-panel">';
  $output .= '<input class="search treedropdownfield-search" type="text" value="" placeholder="Press enter to search" data-skip-autofocus="true"></input>';
  $output .= '<div class="tree-holder"></div>';
  $output .= '</div>';


  $output .= '</div>';
  return $output;	
} 
