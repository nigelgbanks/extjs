<?php

/**
 * @file
 *
 * A collection of helper/utility functions for this module.
 */

/**
 * Returns the path to the ExtJS library.
 *
 * @return string
 *  The path to the ExtJS library if found, FALSE otherwise.
 */
function extjs_get_path() {
  static $path;
  if (isset($path)) {
    return $path;
  }
  $path = FALSE;
  if (function_exists('libraries_get_path')) { // Libraries API integration.
    $path = libraries_get_path('extjs');
    if ($path !== FALSE && !file_exists($path)) {  // Libraries API 1.x returns a default path; 2.x returns FALSE.
      $path = FALSE;
    }
  }
  else if (file_exists('./sites/all/libraries/extjs')) {   // Manually check sites/all/libraries in case Libraries API is not available.
    $path = 'sites/all/libraries/extjs';
  }
  return $path;
}

/**
 * Returns the path to the ExtJS library's resource folder where all the css/image/theme data is kept.
 *
 * @return string
 *  The path to the ExtJS library's resouce folder if found, FALSE otherwise.
 */
function extjs_get_resources_path() {
  return extjs_get_path() . '/resources';
}

/**
 * Returns the path to the ExtJS library's core css folder.
 *
 * @return string
 *  The path to the ExtJS library's css folder if found, FALSE otherwise.
 */
function extjs_get_css_path() {
  return extjs_get_resources_path() . '/css';
}

/**
 * Returns the path to the ExtJS library's core js folder.
 *
 * @return string
 *  The path to the ExtJS library's js folder if found, FALSE otherwise.
 */
function extjs_get_js_path() {
  return extjs_get_path();
}

/**
 * Gets the installed version of ExtJS
 *
 * @return string
 *  The currently installed version, if found FALSE otherwise.
 */
function extjs_get_version() {
  static $version;
  if(isset($version)) {
    return $version;
  }
  $version = FALSE;
  $filename = extjs_get_path() . '/ext.js';
  if(file_exists($filename)) {
    $text = file_get_contents($filename);
    $matches = array();
    preg_match('/setVersion\(\"extjs\",\"([^.]*\.[^.]*\.[^.]*)\"\)/', $text, $matches);
    if(isset($matches[1])) { // The first sub expression.
      $version = $matches[1];
    }
  }
  return $version;
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
    EXTJS_DEBUG_COMMENTS_COMPRESSION => '-debug-w-comments.js'
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
    EXTJS_DEBUG_COMMENTS_COMPRESSION => '-debug.css'
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
  drupal_add_js($path . '/' . $file . extjs_get_js_file_suffix());
}