<?php
/*
 * @file
 * Function file to administer the userloginbar module settings.
 */

/**
 * Provides the userloginbar settings form.
 */
function userloginbar_admin_settings() {
  $form = array();
  $form['disable_welcome_box'] = array(
    '#type' => 'checkbox',
    '#title' => t('Check this box if you want to disable welcome box when the user logs in!'),
    '#default_value' => variable_get('disable_welcome_box', FALSE),
  );
  return system_settings_form($form);
}
