<?php
// $Id$

/**
 * @file
 * 	Allow user choise type of inject Closure to Drupal
 */

/**
 * Generate Configuration of Google Closure Library
 * 	User can choise running Google Closure Library with build-in jQuery of Drupal
 * or without jQuery (System will use our port Drupal object to Closure)
 */
function closure_settings() {
  $form = array();
  $form['closure_js_type'] = array(
    '#type' => 'radios',
    '#title' => t('Run Google Closure Library'),
    '#default_value' => variable_get('closure_js_type', CLOSURE_WITH_JQUERY),
    '#options' => array(
      CLOSURE_WITH_JQUERY => t('with jQuery'),
      CLOSURE_WITHOUT_JQUERY => t('without jQuery'),
    ),
  );
  return system_settings_form($form);
}