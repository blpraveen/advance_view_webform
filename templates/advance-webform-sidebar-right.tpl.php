<div class="webform-advance-sidebar-right">
<h3 class="sidebar-title"><?php echo t('Group Visibility'); ?></h3>
<div class="sidebar-content">
<?php if($is_shared) { ?>
<p>Owner has perrmission to set the users not others</p>
<?php } else if(!$groupid) { ?>
<p>You can add group permission to any group. You need to select group.</p>
<?php } else  { ?>
<?php print $access_form; ?>
<?php } ?>
</div>
</div>
