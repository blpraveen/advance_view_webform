<div class="webform-advance-sidebar">
<ul class="webform-group-list">
	<li class="group-item"><a href="?q=admin/content/webform/advanced/group/">My Webforms</a>
		<?php if(!$is_shared) { ?>
			<?php if(is_array($group_sub) && count($group_sub)) { ?>
			<ul class="group-sub-menu">

				<?php foreach($group_sub as $group) { ?>
					<li><a href="?q=admin/content/webform/advanced/group/<?php echo $group->gid; ?>"><?php print $group->name ?></a></li>		
				<?php } ?>

			</ul>
			<?php }  else { ?>
				<ul class="group-sub-menu">
					<li><p>No Groups. Create It</p></li>
				</ul>
					
			<?php }   ?>
		<?php }   ?>
	</li>
	<li class="group-item"><a href="?q=admin/content/webform/advanced/group/shared/">Shared With Me</a>
		<?php if($is_shared) { ?>
			<?php if(is_array($group_sub) && count($group_sub)) { ?>
			<ul class="group-sub-menu">
			
				<?php foreach($group_sub as $group) { ?>
					<li><a href="?q=admin/content/webform/advanced/group/shared/<?php echo $group->gid; ?>"><?php print $group->name ?></a></li>		
				<?php } ?>
		
			</ul>
			<?php } else { ?>
				<ul class="group-sub-menu">
					<li><p>No Shared Groups</p></li>
				</ul>
			<?php }   ?>
		<?php }   ?>
	</li>
</ul>
</div>
