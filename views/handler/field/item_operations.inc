<?php

/**
 * @file
 * View handler to remove an UiTDB item from a list.
 */

use \CultuurNet\Search\ActivityStatsExtendedEntity;

/**
 * Field handler to add operations to an item in the lists.
 */
class CulturefeedListsViewsHandlerFieldItemOperations extends views_handler_field {

  /**
   * Render the management links.
   *
   * @param ActivityStatsExtendedEntity|mixed $extended_entity
   *   The extended search entity.
   *
   * @return string
   *   The links render.
   */
  public function render($extended_entity) {
    if (!$extended_entity instanceof ActivityStatsExtendedEntity) {
      return '';
    }

    // Only active if user has access to administer lists.
    if (!user_access(CULTUREFEED_LISTS_PERMISSION_ADMINISTER)) {
      return '';
    }

    // Only active on management pages.
    $list = culturefeed_lists_menu_get_list();
    if (!$list) {
      return '';
    }

    $links = array();

    // View the item.
    $links['view'] = l(
      t('View'),
      culturefeed_entry_ui_entity_path($extended_entity, 'view')
    );

    // Manage the item.
    $links['manage'] = l(
      t('Manage'),
      culturefeed_entry_ui_entity_path($extended_entity, 'lists')
    );

    // Remove from the list.
    $links['remove'] = l(
      t('Remove from list'),
      culturefeed_lists_admin_path(
        array($list->getId(), 'manage', $extended_entity->getId(), 'remove')
      )
    );

    // Render output.
    if (empty($links)) {
      return '';

    }

    return implode('', $links);
  }

}
