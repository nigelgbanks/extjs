<?php

/**
 * @file
 * Handles the display/submission of the admin settings form for this module.
 */

/**
 * Defines the admin settings form.
 *
 * @param array $form
 *   The Drupal form definition.
 * @param array $form_state
 *   The Drupal form state.
 *
 * @return array
 *   The Drupal form definition.
 */
function extjs_admin_settings_form(array $form, array &$form_state) {
  $form = array(
    'extjs_neptune_theme' => array(
      '#type' => 'checkbox',
      '#title' => t('Use the Neptune Theme'),
      '#default_value' => variable_get('extjs_neptune_theme', FALSE),
    ),
    'extjs_compression_type' => array(
      '#type' => 'radios',
      '#title' => t('Choose ExtJS compression level'),
      '#options' => array(
        EXTJS_MIN_COMPRESSION => t('Production (Minified)'),
        EXTJS_DEV_COMPRESSION => t('Development (Raw)'),
        EXTJS_DEBUG_COMPRESSION => t('Development (Debug)'),
        EXTJS_DEBUG_COMMENTS_COMPRESSION => t('Development (Debug with Comments)'),
      ),
      '#default_value' => variable_get('extjs_compression_type', EXTJS_MIN_COMPRESSION),
    ),
    // Clear the javascript cache when the setting is updated.
    '#submit' => array('drupal_clear_js_cache'),
  );
  return system_settings_form($form);
}
