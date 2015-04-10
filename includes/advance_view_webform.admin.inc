<?php


/**
 * Menu callback for admin/content/webform. Displays all webforms on the site.
 */

function advance_view_webform_admin_advance_view_group_shared($groupid = FALSE) {
   return theme('advance_view_webform_admin_advance_view_group_shared', array('group_id' => $groupid));
}
function advance_view_webform_admin_advance_view_group( $groupid = FALSE) {
   return theme('advance_view_webform_admin_advance_view_group', array('group_id' => $groupid));
}
function  advance_view_webform_admin_advance_content() {
  global $user;
  $header_order = array(
	 array('data' => 'Nid', 'field' => 'n.nid',  'sort' => 'DESC'),
	 array('data' => 'Title', 'field' => 'n.title'),
  );

 
  drupal_add_js($modal_style, 'setting');
  //$filters = drupal_get_form('webform_advance_filter_form');
  drupal_add_js(drupal_get_path('module', 'advance_view_webform'). '/js/jquery.colorbox.js');
  drupal_add_js(drupal_get_path('module', 'advance_view_webform'). '/js/advance_view_webform.js');
  $filter_val =  isset($_SESSION['advance_webform']) ? $_SESSION['advance_webform'] : false;
  $query = db_select('webform', 'w')->extend('TableSort');
  $query->join('node', 'n', 'w.nid = n.nid');
  $query->fields('n');
  $query->orderByHeader($header_order);
  $query->orderBy('nid', 'DESC');

  drupal_add_css(drupal_get_path('module', 'advance_view_webform') . '/css/webform-view.css');
  $nodes = $query->execute()->fetchAllAssoc('nid');
  $query = db_select('advance_webform_group', 'g');
  $query->fields('g');
  $query->condition('g.uid',$user->uid, '=');
  $groupnodes = $query->execute()->fetchAllAssoc('gid');
  //$action_links['add-group'] = array('#markup' => '<li><a href="admin/advgrp/add/modal/nojs/">Add Group</a></li>');
  $action_links['add-group'] = array('#markup' =>  t('<a href="@href" class="aw-colobox-modal">@name</a>', array('@href' => "?q=advance/webform/add",'@name' => t('Add New Group'))));
  //return theme('advance_webform_view', array('nodes' => $nodes , 'filters' =>   $filters));
  return theme('advance_webform_view', array('nodes' => $nodes,'groupnodes' =>   array(),'action_links' => $action_links));
}
/**
 * Generate a filter form.
 */
function webform_advance_filter_form(){
  $filter_val =  isset($_SESSION['advance_webform']) ? $_SESSION['advance_webform'] : false;
   $title = '';
   
  if (isset($filter_val['filter_title'])) { 
	$title = $filter_val['filter_title'];
   }
  $form['filters'] = array(
    '#type' => 'fieldset',
    '#title' => t('Show only Webforms where'),
    '#theme' => 'exposed_filters__user',
  );
  $form['filters']['filter_title'] = array(
    '#weight' => '1',
    '#required' => '0',
    '#type' => 'textfield',
    '#size' => 25,
    '#title' => t('WebForm Title'),
    '#default_value' => $title,
  );


  $form['filters']['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Filter',
    '#weight' => '2',
  );
   $form['filters']['reset'] = array(
    '#type' => 'submit',
    '#value' => 'Reset',
    '#weight' => '3',
 );

  return $form;
}

function theme_advance_view_webform_admin_advance_view_group_shared($variables) {
  $form = drupal_get_form('advance_webform_add_group_form',NULL,$variables['group_id']);
  $grpid = ($variables['group_id']) ?$variables['group_id'] : 0;
  $output = '';
  //$filter_val =  isset($_SESSION['advance_webform']) ? $_SESSION['advance_webform'] : false;

  $query = db_select('webform', 'w')->extend('TableSort');
  $query->join('node', 'n', 'w.nid = n.nid');
  $query->fields('n'); 
  if($grpid > 0) {
	$query->condition('w.advance_group_id',$grpid, '=');
  }
  $query->orderByHeader($header_order);
  $query->orderBy('nid', 'DESC');
  drupal_add_js(drupal_get_path('module', 'advance_view_webform'). '/js/advance_view_webform.js');
  drupal_add_css(drupal_get_path('module', 'advance_view_webform') . '/css/webform-view.css');
  $nodes = $query->execute()->fetchAllAssoc('nid');
  $query = db_select('advance_webform_group', 'g');
  $query->fields('g');
  $query->condition('g.uid',$user->uid, '=');
  $groupnodes = $query->execute()->fetchAllAssoc('gid');
  //$groups = advance_view_module_groups($grpid );
  $groups = advance_view_module_shared_groups($grpid);
  $output .= theme('advance_webform_view', array('nodes' => $nodes,'is_shared' => true, 'groupnodes' =>   $groups,'form' => drupal_render($form), 'groupid' => $grpid,'access_form' => drupal_render($access_form)));

  return $output;
}

function theme_advance_view_webform_admin_advance_view_group($variables) {

  $form = drupal_get_form('advance_webform_add_group_form',NULL,$variables['group_id']);
  $access_form = drupal_get_form('advance_webform_group_access_webforms',$variables['group_id']);
  $grpid = ($variables['group_id']) ?$variables['group_id'] : 0;
  $output = '';
  //$filter_val =  isset($_SESSION['advance_webform']) ? $_SESSION['advance_webform'] : false;

  $query = db_select('webform', 'w')->extend('TableSort');
  $query->join('node', 'n', 'w.nid = n.nid');
  $query->fields('n'); 
  if($grpid > 0) {
	$query->condition('w.advance_group_id',$grpid, '=');
  }
  $query->orderByHeader($header_order);
  $query->orderBy('nid', 'DESC');
  drupal_add_js(drupal_get_path('module', 'advance_view_webform'). '/js/advance_view_webform.js');
  drupal_add_css(drupal_get_path('module', 'advance_view_webform') . '/css/webform-view.css');
  $nodes = $query->execute()->fetchAllAssoc('nid');
  $query = db_select('advance_webform_group', 'g');
  $query->fields('g');
  $query->condition('g.uid',$user->uid, '=');
  $groupnodes = $query->execute()->fetchAllAssoc('gid');
  //$groups = advance_view_module_groups($grpid );
  $groups = advance_view_module_sub_groups($grpid);
  $output .= theme('advance_webform_view', array('nodes' => $nodes,'is_shared' => false, 'groupnodes' =>   $groups,'form' => drupal_render($form), 'groupid' => $grpid,'access_form' => drupal_render($access_form)));

  return $output;
}
/**
 * Generate a list of all webforms avaliable on this site.
 */
function theme_advance_webform_view($variables) {
  $nodes = $variables['nodes'];
  $filters = $variables['filters'];
  $header = array(
   array('data' =>  t('Title'),'field' => 'title','sort' => 'ASC'),
    array('data' => t('View'), 'colspan' => '4'),
    array('data' => t('Operations'), 'colspan' => '3')
  );

  $rows = array();
  foreach ($nodes as $node) {

    $rows[] = array(
      l($node->title, 'node/' . $node->nid),
      l(t('Submissions'), 'node/' . $node->nid . '/webform-results'),
      l(t('Analysis'), 'node/' . $node->nid . '/webform-results/analysis'),
      l(t('Table'), 'node/' . $node->nid . '/webform-results/table'),
      l(t('Download'), 'node/' . $node->nid . '/webform-results/download'),
      node_access('update', $node) ? l(t('Edit'), 'node/' . $node->nid . '/edit') : '',
      node_access('update', $node) ? l(t('Components'), 'node/' . $node->nid . '/webform') : '',
      user_access('delete all webform submissions') ? l(t('Clear'), 'node/' . $node->nid . '/webform-results/clear') : '',
    );
  }
  
  if (empty($rows)) {
    $webform_types = webform_variable_get('webform_node_types');
    if (empty($webform_types)) {
      $message = t('Webform is currently not enabled on any content types.') . ' ' . t('Visit the <a href="!url">Webform settings</a> page and enable Webform on at least one content type.', array('!url' => url('admin/config/content/webform')));
    }
    else {
      //$webform_type_list = webform_admin_type_list();
      $message = t('There are currently no webforms on your site. Add a webforms to this group.');
    }

    $rows[] = array(
      array('data' => $message, 'colspan' => 7),
    );
  }
 

  $output = array();
  $output['filters'] = $filters;
  $output['sidebar'] = array(
      '#name' => array('Hello'),
      '#theme' => 'advance_webform_sidebar',
  );
  $output['table'] = array(
      '#header' => $header,
      '#rows' => $rows,
      '#theme' => 'table',
  );
  //$output['attributes'] = array('id' => 'group-table');
  //drupal_add_tabledrag('group-table', 'order', 'sibling', 'slides-order-weight');
  $output['pager'] = array(
      '#theme' => 'pager',
  );
  return $output;
}

function theme_advance_webform_table($variables) {
  $nodes = $variables['nodes'];
 // $filters = $variables['filters'];
  $header = array(
   array('data' =>  t('Title'),'field' => 'title','sort' => 'ASC'),
    array('data' => t('View'), 'colspan' => '4'),
    array('data' => t('Operations'), 'colspan' => '3')
  );

  $rows = array();
  foreach ($nodes as $node) {

    $rows[] = array(
      l($node->title, 'node/' . $node->nid),
      l(t('Submissions'), 'node/' . $node->nid . '/webform-results'),
      l(t('Analysis'), 'node/' . $node->nid . '/webform-results/analysis'),
      l(t('Table'), 'node/' . $node->nid . '/webform-results/table'),
      l(t('Download'), 'node/' . $node->nid . '/webform-results/download'),
      node_access('update', $node) ? l(t('Edit'), 'node/' . $node->nid . '/edit') : '',
      node_access('update', $node) ? l(t('Components'), 'node/' . $node->nid . '/webform') : '',
      user_access('delete all webform submissions') ? l(t('Clear'), 'node/' . $node->nid . '/webform-results/clear') : '',
    );
  }
  
  if (empty($rows)) {
    $webform_types = webform_variable_get('webform_node_types');
    if (empty($webform_types)) {
      $message = t('Webform is currently not enabled on any content types.') . ' ' . t('Visit the <a href="!url">Webform settings</a> page and enable Webform on at least one content type.', array('!url' => url('admin/config/content/webform')));
    }
    else {
      //$webform_type_list = webform_admin_type_list();
      $message = t('There are currently no webforms on your site. Add a webforms to this group.');
    }

    $rows[] = array(
      array('data' => $message, 'colspan' => 7),
    );
  }
 

  $output = array();
  $output['table'] = array(
      '#header' => $header,
      '#rows' => $rows,
      '#theme' => 'table',
      '#empty' => t('No group available. <a href="@link">Add content type</a>.', array('@link' => url('#'))),
  );
  return $output;
}

function theme_advance_webform_group($variables) {
  $groups = $variables['groupnodes'];
  $header = array(
   array('data' =>  t('Title'),'field' => 'title','sort' => 'ASC'),
    array('data' => t('Owner')),
    array('data' => t('Last Modified')),
    array('data' => t('Operations'),'colspan' => '2')
  );
  
  $rows = array();
  foreach ($groups as $group) {
    $rows[] = array(
      l($group->name,'admin/content/webform/advanced/group/'.$group->gid),
      db_query("SELECT name FROM {users} WHERE uid = :id",array(':id' => $group->uid))->fetchField(),
      date('d/m/Y', strtotime($group->last_modified)),
      advance_webform_group_access('edit own webform group', $node->nid) ? l(t('Edit'), 'admin/content/webform/advanced/group/edit/' . $group->gid . '/') : '',
      advance_webform_group_access('delete own webform group', $node->nid) ? l(t('Delete'), 'admin/content/webform/advanced/group/delete/' . $group->gid . '/') : '',
    );
  }
  $rows[] = array(
 );
  $output['table'] = array(
      '#header' => $header,
      '#rows' => $rows,
      '#theme' => 'table',
      '#empty' => t('No group available. <a href="@link">Add content type</a>.', array('@link' => url('#'))), 
  );
  return $output;
}
function theme_advance_webform_group_shared($variables) {
  $groups = $variables['groupnodes'];
  $header = array(
   array('data' =>  t('Title'),'field' => 'title','sort' => 'ASC'),
    array('data' => t('Owner')),
    array('data' => t('Last Modified'))
  );
  
  $rows = array();
  foreach ($groups as $group) {
    $rows[] = array(
      l($group->name,'admin/content/webform/advanced/group/shared/'.$group->gid),
      db_query("SELECT name FROM {users} WHERE uid = :id",array(':id' => $group->uid))->fetchField(),
      date('d/m/Y', strtotime($group->last_modified))
    );
  }
  $rows[] = array(
 );
  $output['table'] = array(
      '#header' => $header,
      '#rows' => $rows,
      '#theme' => 'table',
      '#empty' => t('No group available. <a href="@link">Add content type</a>.', array('@link' => url('#'))), 
  );
  return $output;
}
function webform_advance_filter_form_submit($form, &$form_state) {
    /*if($form_state['values']['op'] == 'Reset') {
	    form_state_values_clean($form_state);
	    variable_del('filter_title');
	    unset($_SESSION['advance_webform']);
    } else {	
	    $form_state['filters']['filter_title'] = $form_state['values']['filter_title'];
	    $_SESSION['advance_webform'] = $form_state['filters'];
    }
    $form_state['redirect'] = url(current_path(), array(
	'absolute' => TRUE
	)
    );*/
}



function advance_webform_group_add($page_callback_result) {
	echo drupal_render(drupal_get_form('advance_webform_add_group_form'));
}

function advance_webform_group_submit($form, &$form_state,$args) {
  global $user;
  $op = $form_state['values']['op'];
 if ($op == t('Save Group') ) {
	$group = advance_view_module_get_group(arg(6));
	if($group && $group->uid == $user->uid) {
		 $editgroup = new stdClass;
		 $editgroup->name = $form_state['input']['group_name'];
		 $editgroup->pid = 0; 
		 $editgroup->weight = 0; 
		 if(isset($form_state['input']['group_parent'])) {
		 	$editgroup->pid = (int) $form_state['input']['group_parent'];
		 }
		 $editgroup->uid = $user->uid;
		 advance_view_module_save_group($editgroup,$group->gid);	
	}
  } else if($op == t('Add Group')) {	
	    $group = new stdClass;
	    $group->name = $form_state['values']['group_name'];
            $group->pid = 0; 
	    $group->weight = 0; 
	    if(isset($form_state['input']['group_parent'])) {
	 	$group->pid = (int) $form_state['input']['group_parent'];
	    }
	    $group->uid =  $user->uid; 
	    advance_view_module_save_group($group);	
	 
  } else if($op == t('Add Group')) {	
	    $group = new stdClass;
	    $group->name = $form_state['values']['group_name'];
            $group->pid = 0; 
	    $group->weight = 0; 
	    if(isset($form_state['input']['group_parent'])) {
	 	$group->pid = (int) $form_state['input']['group_parent'];
	    }
	    $group->uid =  $user->uid; 
	    advance_view_module_save_group($group);	
	 
  }
  menu_rebuild();
  return;
}
function advance_webform_group_edit($group) {
	echo $group;
}

/**
 * Validation functino for the admin page form.
 */
function advance_webform_group_validate($form, &$form_state) {

    // Make sure a group name is present
    if (empty($form_state['values']['group_name'])) {
      form_set_error('title', t('You must enter a page Group name.'));
    }
    if (empty($form_state['values']['group_parent'])) {
      form_set_error('title', t('Something is wrong while editing. Please try again'));
    }
}

/**
 * Generate a filter form.
 */
function advance_webform_add_group_form($form, &$form_state, $groupid = FALSE,$gpid = FALSE){
  $group = advance_view_module_get_group($groupid);

  if (empty($group)) {
    $form['group'] = array(
      '#type' => 'fieldset',
      '#title' => t('Add a new Group'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
  }
  $form['group']['group_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Group name'),
    '#required' => TRUE,
    '#size' => 32,
    '#maxlength' => 64,
    '#description' => t('New Group Name'),
  );
  $form['group']['group_parent'] = array(
	'#type' => 'hidden',
	'#value' => 0,
  );
  if(empty($group) && $gpid) {
	$form['group']['group_parent']['#value'] = $gpid;
  }
  if ($group) {
    $form['group']['group_parent']['#value'] = $group->pid;	
    $form['group']['group_name']['#value'] = $group->name;
    $form['group']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save Group'),
    );
  } else {

    $form['group']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Add Group'),
    );
  }
    $form['#submit'][] = 'advance_webform_group_submit';
    $form['#validate'][] = 'advance_webform_group_validate';
 
  return $form;
}



/**
 * Per Group Webform Access Settings .
 */
function advance_webform_group_access_webforms($form, &$form_state, $groupid) {
  $group = advance_view_module_get_group($groupid);
  //drupal_set_title(t('Access control for @title', array('@title' => $group->name)));

  if(empty($group)) return;

  $users = advance_view_module_get_group_access_users($groupid);
  
  /*
  foreach (_content_access_get_operations() as $op => $label) {
    $defaults[$op] = content_access_per_node_setting($op, $node);
  }*/

  // Get roles form
  //content_access_role_based_form($form, $defaults, $node->type);

  // Add an after_build handler that disables checkboxes, which are enforced by permissions.
  //$form['per_role']['#after_build'] = array('content_access_force_permissions');

  // ACL form
  /*if (module_exists('acl')) {
    // This is disabled when there is no node passed.
    $form['acl'] = array(
      '#type' => 'fieldset',
      '#title' => t('User access control lists'),
      '#description' => t('These settings allow you to grant access to specific users.'),
      '#collapsible' => TRUE,
      '#tree' => TRUE,
    );

    foreach (array('view', 'update', 'delete') as $op) {
      $acl_id = content_access_get_acl_id($node, $op);
      acl_node_add_acl($node->nid, $acl_id, (int) ($op == 'view'), (int) ($op == 'update'), (int) ($op == 'delete'), content_access_get_settings('priority', $node->type));

      $form['acl'][$op] = acl_edit_form($form_state, $acl_id, t('Grant !op access', array('!op' => $op)));
      $form['acl'][$op]['#collapsed'] = !isset($_POST['acl_' . $acl_id]) && !unserialize($form['acl'][$op]['user_list']['#default_value']);
    }
  }*/
   $form['group_access'] = array(
      '#type' => 'fieldset',
      '#title' => t('User access control lists'),
      '#description' => t('These settings allow you to grant access to specific users.'),
      '#collapsible' => TRUE,
    );
  $form['group_access']['deletions'] = array(
    '#type' => 'checkboxes',
    '#options' => array(),
    '#validate' => TRUE
  ); // placeholder
  $form['group_access']['delete_button'] = array(
    '#type' => 'submit',
    '#name' => 'group_' . $groupid,
    '#value' => t('Remove Checked'),
  );

  $form['group_access']['add'] = array(
    '#type' => 'textfield',
    '#title' => t('Add user'),
    '#maxlength' => 60,
    '#size' => 28,
    '#autocomplete_path' => 'user/autocomplete',
  );

  $form['group_access']['add_button'] = array(
    '#type' => 'submit',
    '#name' => 'group_' . $groupid,
    '#value' => t('Add User'),
    //'#submit' => 'advance_webform_group_access_submit',
    //'#validate' => 'advance_webform_group_access_validate'
  );
   $form['group_access']['user_list'] = array(
    '#type' => 'hidden',
    '#default_value' => serialize($users),
  );
  $form['#submit'][] = 'advance_webform_group_access_submit';
  $form['#validate'][] = 'advance_webform_group_access_validate';
  $form['#after_build'] = array('advance_webform_group_access_webforms_after_build');
  return $form;
}





/**
 * Validation functino for the admin page form.
 */
function advance_webform_group_access_validate($form, &$form_state) {
   $grpid = arg(5);
   if($grpid < 1) {
	form_set_error('title', t('Invalid Group.'));
	return '';
    }
   $group = advance_view_module_get_group($grpid);
    if(empty($group)) { 
	form_set_error('title', t('Invalid Group.'));
	return '';
    }
   if(!isset($form_state['values']['group_'.$grpid])) {
	form_set_error('group_'.$grpid, t('Invalid Operation.'));
   }
   if($form_state['values']['group_'.$grpid] == 'Add User') {
	if(empty($form_state['values']['add'])) {
		form_set_error('add', t('Please enter user name.'));
	}
	if (!db_query("SELECT COUNT(*) FROM {users} WHERE name = :name;", array(':name' => $form_state['values']['add']))->fetchField()) {
		form_set_error('add', t('Username is invalid.'));
	}
   }
   if($form_state['values']['group_'.$grpid] == 'Remove Checked') {
	if(!count($form_state['values']['deletions'])) {	
		form_set_error('add', t('No user selected.'));
	}
   }
}



function advance_webform_group_access_submit($form, &$form_state,$args) {
  global $user;
  $grpid = arg(5);
  $group = advance_view_module_get_group($grpid);
  if(empty($group)) return '';	
  if ($form_state['values']['group_'.$grpid] == 'Add User' ) {

	 $uid = db_query("SELECT uid FROM {users} WHERE name = :name;", array(':name' => $form_state['values']['add']))->fetchField();
	 if(!$uid) return;
	 $groupaccess = new stdClass;
	 $groupaccess->uid =  $uid;
	 $groupaccess->gid = $grpid;
	 $access_group = advance_webform_group_children($grpid, $uid);
	 advance_view_module_save_group_access(array_merge(array($groupaccess),$access_group),true);
	
  } else if($form_state['values']['group_'.$grpid] == 'Remove Checked') {	
	 advance_view_module_remove_group_access($form_state['values']['deletions']);
  }
  menu_rebuild();
  return;
}

function advance_webform_group_children($grpid,$uid) {
    $child_group = array();
    if ($grpid > 0 ) {
	$subgroups = advance_view_module_sub_groups($grpid);
        foreach ($subgroups as $group) {
	   $groupaccess = new stdClass;
	   $groupaccess->uid =  $uid;
	   $groupaccess->gid = $group->gid;
	   $child_group[] =  $groupaccess;
           $child_result = advance_webform_group_children($group->gid,$uid);
	   if(!empty($child_result)) {
         	$child_group =array_merge($child_group,$child_result);
	   }
        }
    }
    return $child_group;
}
/**
 * Process a form that had our buttons on it.
 */
function advance_webform_group_access_webforms_after_build($form, &$form_state) {
  // We can't use the form values because it's the entire structure
  // and we have no clue where our values actually are. That's
  // ok tho cause #value still works for us.
  $user_list = unserialize($form['group_access']['user_list']['#value']);

  if (isset($form_state['triggering_element']) && $form_state['triggering_element']['#value'] == $form['group_access']['delete_button']['#value']) {
    $deletions = $form['group_access']['deletions']['#value'];
    foreach ($deletions as $uid) {
      unset($user_list[$uid]);
      unset($form['group_access']['deletions']['#value'][$uid]);
    }
  }
  /*elseif (isset($form_state['triggering_element']) && $form_state['triggering_element']['#value'] == $form['add_button']['#value'] && !empty($form['add']['#value'])) {
    $user = db_query("SELECT uid, name FROM {users} WHERE name = :name", array(
      'name' => $form['group_access']['add']['#value'],
    ))->fetchObject();
    if (!$user || !$user->uid) {
      form_error($form['group_access']['add'], t("Invalid user specified."));
    }
    else {
      $user_list[$user->uid] = $user->name;
      $form['group_access']['add']['#value'] = NULL;
    }
  }*/
  if (count($user_list) != 0) {
    $form['group_access']['deletions']['#type'] = 'checkboxes';
    $form['group_access']['deletions']['#title'] = t("Current users");
    $form['group_access']['deletions']['#options'] = $user_list;
    $form['group_access']['deletions']['#validate'] = TRUE;
    $form['group_access']['deletions']['#value'] = array(); // don't carry value through.
    $form['group_access']['deletions'] = form_builder(!empty($form['#post']) ? $form['#post']['form_id'] : 'group_acl_form', $form['group_access']['deletions'], $form_state);
  }
  else {
    unset($form['group_access']['deletions']);
    $form['group_access']['delete_button']['#type'] = 'value';
  }
  $form['group_access']['user_list']['#value'] = serialize($user_list);

  return $form;
}
