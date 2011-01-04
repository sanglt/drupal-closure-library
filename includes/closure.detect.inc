<?php
// $Id$

/**
 * @file
 * 	Detect Closure library on system
 */

/**
 * Detect Google Library on system
 */
function closure_detect_verion() {
  // @TODO: Check closure compile file
  $closure = closure_get_path('closure-library');
  if (file_exists($closure . '/closure/goog/base.js')) {
    return array(
      TRUE,
      '1.0', // @TODO: check Closure version
    );
  }
  else {
    return array(
      FALSE,
      $closure,
    );
  }
}

/**
 * Helper function to get base file of Closure Library
 */
function closure_get_base_file() {
  return closure_get_path('closure-library') . '/closure/goog/base.js';
}


/**
 * Helper function to build paths to libraries.
 *
 * @param $library
 *   The external library name to return the path for.
 * @param $base_path
 *   Whether to prefix the resulting path with base_path().
 *
 * @return
 *   The path to the specified library.
 *
 * @ingroup libraries
 */
function closure_get_path($library, $base_path = FALSE) {
  static $libraries;

  if (!isset($libraries)) {
    $libraries = closure_get_libraries();
  }
  if (!isset($libraries[$library])) {
    // Most often, external libraries can be shared across multiple sites.
    return 'sites/all/libraries/' . $library;
  }

  $path = ($base_path ? base_path() : '');
  $path .= $libraries[$library];

  return $path;
}


/**
 * Return an array of library directories.
 *
 * Returns an array of library directories from the all-sites directory
 * (i.e. sites/all/libraries/), the profiles directory, and site-specific
 * directory (i.e. sites/somesite/libraries/). The returned array will be keyed
 * by the library name. Site-specific libraries are prioritized over libraries
 * in the default directories. That is, if a library with the same name appears
 * in both the site-wide directory and site-specific directory, only the
 * site-specific version will be listed.
 *
 * @return
 *   A list of library directories.
 *
 * @ingroup libraries
 */
function closure_get_libraries() {
  global $profile;

  // When this function is called during Drupal's initial installation process,
  // the name of the profile that is about to be installed is stored in the
  // global $profile variable. At all other times, the regular system variable
  // contains the name of the current profile, and we can call variable_get()
  // to determine the profile.
  if (!isset($profile)) {
    $profile = variable_get('install_profile', 'default');
  }

  $directory = 'libraries';
  $searchdir = array();
  $config = conf_path();

  // The 'profiles' directory contains pristine collections of modules and
  // themes as organized by a distribution.  It is pristine in the same way
  // that /modules is pristine for core; users should avoid changing anything
  // there in favor of sites/all or sites/<domain> directories.
  if (file_exists("profiles/$profile/$directory")) {
    $searchdir[] = "profiles/$profile/$directory";
  }

  // Always search sites/all/*.
  $searchdir[] = 'sites/all/' . $directory;

  // Also search sites/<domain>/*.
  if (file_exists("$config/$directory")) {
    $searchdir[] = "$config/$directory";
  }

  // Retrieve list of directories.
  $directories = array();
  $nomask = array('CVS', '.git');
  foreach ($searchdir as $dir) {
    if (is_dir($dir) && $handle = opendir($dir)) {
      while (FALSE !== ($file = readdir($handle))) {
        if (!in_array($file, $nomask) && $file[0] != '.') {
          if (is_dir("$dir/$file")) {
            $directories[$file] = "$dir/$file";
          }
        }
      }
      closedir($handle);
    }
  }

  return $directories;
}