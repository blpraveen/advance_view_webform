<?php if(count($items)) { ?>
	<ul>
		<?php foreach($items as $item) { ?>
			<li class="<?php echo ($item->has_child) ? 'has-child' : 'no-child'; ?>">
				<a class="" href="" data-gid="<?php echo $item->gid; ?>" data-name="<?php echo $item->name; ?>">
					<?php echo $item->name; ?> 
				</a>
				<?php echo $item->child_output; ?>
			</li>
		<?php }?>
	</ul>
<?php }  ?>
