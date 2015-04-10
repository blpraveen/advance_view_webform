<div class="webform-sidebar">
	<?php echo theme("advance_webform_sidebar",array('group_sub' => $groupnodes ,'is_shared' => $is_shared , 'groupid' => $groupid)); ?>
</div>
<div class="webform-main">
	<?php if(!$is_shared) { ?>
        	<?php print $form; ?>	
		<h2><?php echo t('My Groups'); ?></h2>
		<?php echo drupal_render(theme("advance_webform_group",array('groupnodes' => $groupnodes))); ?>
		<h2><?php echo t('My Webforms'); ?></h2>
		<?php echo drupal_render(theme("advance_webform_table",array('nodes' => $nodes))); ?>
	<?php } else { ?>
		<h2><?php echo t('Shared Groups'); ?></h2>
		<?php echo drupal_render(theme("advance_webform_group_shared",array('groupnodes' => $groupnodes))); ?>
		<?php if($groupid) { ?>
			<h2><?php echo t('Shared Webforms'); ?></h2>
			<?php echo drupal_render(theme("advance_webform_table",array('nodes' => $nodes))); ?>
		<?php }  ?>
	<?php }  ?>

</div>
<div class="webform-sidebar-right">
	<?php echo theme("advance_webform_sidebar_right",array('access_form' => $access_form ,'is_shared' => $is_shared, 'groupid' => $groupid)); ?>
</di)); ?>
</div>
