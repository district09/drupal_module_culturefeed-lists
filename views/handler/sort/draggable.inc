<?php

/**
 * @file
 * View sort handler.
 */

/**
 * Views handler to sort a view b the UDB Item weight.
 */
class CulturefeedListsViewsHandlerSortDraggable extends views_handler {

  /**
   * {@inheritdoc}
   */
  // @codingStandardsIgnoreStart
  public function can_expose() {
  // @codingStandardsIgnoreEnd
    return FALSE;
  }

  /**
   * {@inheritdoc}
   */
  public function query() {
    // No query required.
    // The actual ordering will be performend by a hook_views_pre_render().
    // @see culturefeed_lists_views_pre_render().
  }

  /**
   * {@inheritdoc}
   */
  // @codingStandardsIgnoreStart
  public function admin_summary() {
  // @codingStandardsIgnoreEnd
    return t('List order.');
  }

}
