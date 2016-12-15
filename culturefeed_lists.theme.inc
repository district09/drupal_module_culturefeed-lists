<?php

/**
 * @file
 * Theme related callbacks.
 */

/**
 * Render the help text.
 *
 * @param array $variables
 *   The theme function variables:
 *   - text : array of texts to render in the help.
 *
 * @return string
 *   The rendered help text.
 */
function theme_culturefeed_lists_help($variables) {
  return '<p>'
    . implode('</p><p>', $variables['text'])
    . '</p>';
}
