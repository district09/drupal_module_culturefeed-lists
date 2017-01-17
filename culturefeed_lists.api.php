<?php

/**
 * @file
 * Hook documentation for culturefeed_lists module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Alter the admin path where the list administration is hosted.
 *
 * The hooks can be put in the yourmodule.module OR in the
 * yourmodule.culturefeed.inc file.
 * The recommended place is in the yourmodule.culturefeed.inc file as it keeps
 * your .module file cleaner and makes the platform load less code by default.
 *
 * @param string $path
 *   The path that can be altered.
 * @param array $parts
 *   The parts that are added after the base path.
 */
function hook_culturefeed_lists_admin_path_alter(&$path, array $parts = array()) {
  // Replace the base path by a custom path.
  $path = 'my/custom/path';

  if ($parts) {
    $path .= '/' . implode('/', $parts);
  }
}

/**
 * Alter the Lists administration overview page.
 *
 * This hook allowes other modules to alter the ourput (render array) of the
 * Lists administration overview page.
 *
 * @param array $output
 *   The render array.
 */
function hook_culturefeed_lists_admin_overview_alter(array &$output) {
  foreach ($output['lists']['#rows'] as $id => $row) {
    $output['lists']['#rows'][$id]['data']['name']['data'] = l(
      $row['data']['name']['data'],
      'agenda/l/' . $row['list']->getPath()
    );
  }
}

/**
 * @} End of "addtogroup hooks".
 */
