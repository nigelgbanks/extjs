<?php

/**
 * @file
 * A collection of helper/utility functions for this module.
 */

/**
 * Returns the path to the ExtJS library.
 *
 * @return string
 *   The path to the ExtJS library if found, FALSE otherwise.
 */
function extjs_get_path() {
  static $path;
  if (isset($path)) {
    return $path;
  }
  $path = libraries_get_path('extjs');
  if ($path !== FALSE && !file_exists($path)) {
    $path = FALSE;
  }
  return $path;
}

/**
 * Returns the path to the ExtJS library's resource folder.
 *
 * All the css/image/theme data is kept in the resource folder.
 *
 * @return string
 *   The path to the ExtJS library's resouce folder if found, FALSE otherwise.
 */
function extjs_get_resources_path() {
  return extjs_get_path() . '/resources';
}

/**
 * Returns the path to the ExtJS library's core css folder.
 *
 * @return string
 *   The path to the ExtJS library's css folder if found, FALSE otherwise.
 */
function extjs_get_css_path() {
  return extjs_get_resources_path() . '/css';
}

/**
 * Returns the path to the ExtJS library's core js folder.
 *
 * @return string
 *   The path to the ExtJS library's js folder if found, FALSE otherwise.
 */
function extjs_get_js_path() {
  return extjs_get_path();
}

/**
 * Gets the installed version of ExtJS.
 *
 * @return string
 *   The currently installed version, if found FALSE otherwise.
 */
function extjs_get_version() {
  static $version;
  if (isset($version)) {
    return $version;
  }
  $version = FALSE;
  $filename = extjs_get_path() . '/ext.js';
  if (file_exists($filename)) {
    $text = file_get_contents($filename);
    $matches = array();
    preg_match('/setVersion\(\"extjs\",\"([^.]*\.[^.]*\.[^.]*)\"\)/', $text, $matches);
    // The first sub expression.
    if (isset($matches[1])) {
      $version = $matches[1];
    }
  }
  return $version;
}


/**
 * Generates a version requirement for the extjs library.
 *
 * For use in the hook_requirements() function. This should be used by modules
 * that require the extjs library to have a specific version higher than 4.0.
 *
 * @param string $module_name
 *   The name of the module with the requirement.
 * @param string $required_version
 *   The version the module requries.
 *
 * @return array
 *   A requirement definition for use in the hook_requirements() function.
 *   It should be added to the $requirements array with a unique key.
 */
function extjs_get_version_requirement($module_name, $required_version) {
  $t = get_t();
  $requirement = array('title' => $t('Ext JS Library'));
  $requirement['value'] = $version = extjs_get_version();
  if (version_compare($version, $required_version) >= 0) {
    $requirement['severity'] = REQUIREMENT_OK;
  }
  else {
    $requirement['severity'] = REQUIREMENT_ERROR;
    $requirement['description'] = extjs_get_version_requirement_status_message($module_name, $required_version);
  }
  return $requirement;
}

/**
 * Returns the 'de facto' required description message.
 *
 * This can be used by modules that require a specific version of Ext JS.
 *
 * @param string $module
 *   The name of the module with the requirement.
 * @param string $version
 *   The version the module requries.
 *
 * @return string
 *   A translated description message that can be used in the status page to
 *   describe which version of extjs is required and where it can be downloaded.
 */
function extjs_get_version_requirement_status_message($module, $version) {
  $t = get_t();
  return $t('The module "@module" requires !extjs version @version or higher. !download and extract it into the !directory directory. Rename the extracted folder to !name.',
    array(
      '@module' => $module,
      '@version' => $version,
      '!extjs' => '<a href="http://www.sencha.com/">Ext JS</a>',
      '!download' => '<a href="http://www.sencha.com/products/extjs/download?page=a">' . t('Download') . '</a>',
      '!directory' => '<code>sites/all/libraries</code>',
      '!name' => '<code>extjs</code>',
    )
  );
}

/**
 * Gets the suffix for the file depending on the compression settings.
 *
 * @return string
 *   The suffix of the file, denoting its compression type.
 */
function extjs_get_js_file_suffix() {
  $suffix = array(
    EXTJS_MIN_COMPRESSION => '.js',
    EXTJS_DEV_COMPRESSION => '-dev.js',
    EXTJS_DEBUG_COMPRESSION => '-debug.js',
    EXTJS_DEBUG_COMMENTS_COMPRESSION => '-debug-w-comments.js',
  );
  $compression_type = variable_get('extjs_compression_type', EXTJS_MIN_COMPRESSION);
  return $suffix[$compression_type];
}

/**
 * Gets the suffix for the file depending on the compression settings.
 *
 * @return string
 *   The suffix of the file, denoting its compression type.
 */
function extjs_get_css_file_suffix() {
  $suffix = array(
    EXTJS_MIN_COMPRESSION => '.css',
    EXTJS_DEV_COMPRESSION => '-debug.css',
    EXTJS_DEBUG_COMPRESSION => '-debug.css',
    EXTJS_DEBUG_COMMENTS_COMPRESSION => '-debug.css',
  );
  $compression_type = variable_get('extjs_compression_type', EXTJS_MIN_COMPRESSION);
  return $suffix[$compression_type];
}

/**
 * Includes the given extjs css file.
 */
function extjs_add_css_file($file) {
  $path = extjs_get_css_path();
  drupal_add_css($path . '/' . $file . extjs_get_css_file_suffix(), 'theme', 'all');
}

/**
 * Includes the given extjs js file.
 */
function extjs_add_js_file($file) {
  $path = extjs_get_js_path();
  drupal_add_js($path . '/' . $file . extjs_get_js_file_suffix(), 'module', 'header', TRUE);
}

/**
 * Not safe yet, the theme layer must be spec'd out.
 */
function extjs_add_default_settings($settings = NULL) {
  drupal_add_js(array(
      'extjs' => array(
        'buildSettings' => array(
          'scopeResetCSS' => TRUE))), 'setting');
}

/**
 * Not safe yet, the theme layer must be spec'd out.
 */
function extjs_add_settings_js_file() {
  drupal_add_js(drupal_get_path('module', 'extjs') . '/js/extjs-settings.js', 'module', 'header', TRUE);
}

/**
 * Includes several css files that are used to reset the style.
 *
 * Remove css styles applied to tag without any class qualifiers.
 * Currently this only corrects for the Drupal system module and the
 * Garland theme, custom themes are expected to scope their styles.
 */
function extjs_add_drupal_reset_css_files() {
  $css_path = drupal_get_path('module', 'extjs') . '/css';
  // Fix the Drupal system.css problems. @TODO build theme system for extjs.
  drupal_add_css("$css_path/reset-system.css", 'theme', 'all');
  drupal_add_css("$css_path/reset-defaults.css", 'theme', 'all');
  if (variable_get('theme_default', NULL) == 'garland') {
    // Fix the Garland theme style.css problems.
    drupal_add_css("$css_path/reset-garland-style.css", 'theme', 'all');
  }
}
