<?php

/**
 * @file
 *
 * The admin settings
 */

/**
 * Form definition for the admin page.
 */
function extjs_admin_settings_form(array &$form_state) {
  // Clear the javascript cache when the setting is updated.
  $form['#submit'][] = 'drupal_clear_js_cache';
  $form['extjs_neptune_theme'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use the Neptune Theme'),
    '#default_value' => variable_get('extjs_neptune_theme', FALSE),
  );
  $form['extjs_compression_type'] = array(
    '#type' => 'radios',
    '#title' => t('Choose ExtJS compression level'),
    '#options' => array(
      EXTJS_MIN_COMPRESSION => t('Production (Minified)'),
      EXTJS_DEV_COMPRESSION => t('Development'),
      EXTJS_DEBUG_COMPRESSION => t('Development (Debug)'),
      EXTJS_DEBUG_COMMENTS_COMPRESSION => t('Development (Debug with Comments)'),
    ),
    '#default_value' => variable_get('extjs_compression_type', EXTJS_MIN_COMPRESSION),
  );
  return system_settings_form($form);
}
