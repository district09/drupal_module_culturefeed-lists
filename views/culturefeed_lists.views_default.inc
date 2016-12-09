<?php

/**
 * @file
 * Default view to manage the list items.
 */

/**
 * Implements hook_views_default_views().
 */
function culturefeed_lists_views_default_views() {
  $views = array();

  $view = new view();
  $view->name = 'culturefeed_lists_administer';
  $view->description = 'Overview and management of CultureFeed list items.';
  $view->tag = 'CultureFeed';
  $view->base_table = 'cdb_items';
  $view->human_name = 'CultureFeed : Manage list items';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'CultureFeed : Manage list items';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['sort_asc_label'] = 'Ascending';
  $handler->display->display_options['exposed_form']['options']['sort_desc_label'] = 'Descending';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '30';
  $handler->display->display_options['pager']['options']['expose']['offset_label'] = 'Start';
  $handler->display->display_options['style_plugin'] = 'table';
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = '<h3>There are no items in the list.</h3>';
  $handler->display->display_options['empty']['area']['format'] = 'full_html';
  /* Field: Cdb items: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'cdb_items';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['override'] = 0;
  $handler->display->display_options['fields']['title']['link'] = 1;
  /* Field: Cdb items: Calendar summary */
  $handler->display->display_options['fields']['when']['id'] = 'when';
  $handler->display->display_options['fields']['when']['table'] = 'cdb_items';
  $handler->display->display_options['fields']['when']['field'] = 'when';
  $handler->display->display_options['fields']['when']['label'] = 'Dates';
  /* Field: Cdb Items Lists: Item operations */
  $handler->display->display_options['fields']['lists_item_operations']['id'] = 'link_';
  $handler->display->display_options['fields']['lists_item_operations']['table'] = 'cdb_items';
  $handler->display->display_options['fields']['lists_item_operations']['field'] = 'list_item_operations';
  $handler->display->display_options['fields']['lists_item_operations']['label'] = 'Operations';
  /* Field: Cdb items Lists: Draggable Sort */
  $handler->display->display_options['fields']['draggableviews']['id'] = 'draggableviews';
  $handler->display->display_options['fields']['draggableviews']['table'] = 'cdb_items';
  $handler->display->display_options['fields']['draggableviews']['field'] = 'draggableviews';
  $handler->display->display_options['fields']['draggableviews']['label'] = 'Sort';
  $handler->display->display_options['fields']['draggableviews']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['draggableviews']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['draggableviews']['draggableviews']['handler'] = 'CdbItems';
  $handler->display->display_options['fields']['draggableviews']['draggableviews']['ajax'] = 0;
  /* Sort criterion: Cdb items Lists: Draggable Sort */
  $handler->display->display_options['sorts']['draggableviews']['id'] = 'draggableviews';
  $handler->display->display_options['sorts']['draggableviews']['table'] = 'cdb_items';
  $handler->display->display_options['sorts']['draggableviews']['field'] = 'draggableviews';
  /* Filter criterion: Cdb Items Lists: List */
  $handler->display->display_options['filters']['list_id']['id'] = 'list_id';
  $handler->display->display_options['filters']['list_id']['table'] = 'cdb_items';
  $handler->display->display_options['filters']['list_id']['field'] = 'list_id';
  $handler->display->display_options['filters']['list_id']['value'] = array();
  $translatables['culturefeed_lists_administer'] = array(
    t('Master'),
    t('CultureFeed : Manage list items'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Ascending'),
    t('Descending'),
    t('Items per page'),
    t('- All -'),
    t('Start'),
    t('« first'),
    t('‹ previous'),
    t('next ›'),
    t('last »'),
    t('<h3>There are no items in the list.</h3>'),
    t('Title'),
    t('Dates'),
    t('Operations'),
    t('Sort'),
  );

  $views['culturefeed_lists_administer'] = $view;

  return $views;
}
