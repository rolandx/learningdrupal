<?php
// $Id$

function userreg_school_register_page(){
  $output = '';
  drupal_set_title('');
  $output .= '<div class="register">';
  $output .= t('<h4>以下均为必须填写项</h4>');
  $form = drupal_get_form('userreg_school_register_form');
  $output .= drupal_render($form);
  $output .= '</div>';
  return $output;
}
function userreg_school_register_form($form, &$form_state){
  $form = array();
  global $user;
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('用户名'),
    '#required' => TRUE,
    '#weight' => 96,
  );
  $form['mail'] = array(
    '#type' => 'textfield',
    '#title' => t('电子邮箱'),
    '#required' => TRUE,
    '#weight' => 97,
  );
  $form['pass'] = array(
    '#type' => 'password_confirm',
    //'#title' => t('密码'),
    '#required' => TRUE,
    '#weight' => 98,
  );
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('学校名'),
    '#required' => TRUE,
    '#attributes' => array('class' => array('company_name')),
  );
  $node = (object) array(
    'uid' => $user->uid, 
    'name' => (isset($user->name) ? $user->name : ''), 
    'type' => 'school', 
    'language' => LANGUAGE_NONE
  );

  //$form = drupal_get_form('node_form',$node);
  field_attach_form('node', $node, $form, $form_state);

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('注册'),
  );
  $form['#validate'][] = 'userreg_school_register_form_validate_handler';
  $form['#submit'][] = 'userreg_school_register_form_submit_handler';
  
  return $form;
}
function userreg_school_register_form_validate_handler(&$form, &$form_state) {
  $pseudo_entity = (object) $form_state['values'];
  $pseudo_entity->type = 'school';
  field_attach_form_validate('node', $pseudo_entity, $form, $form_state);
	
  if (isset($form_state['values']['name'])) {
    if ($error = user_validate_name($form_state['values']['name'])) {
      form_set_error('name', $error);
    }
    elseif ((bool) db_select('users')->fields('users', array('uid'))->condition('name', db_like($form_state['values']['name']), 'LIKE')->range(0, 1)->execute()->fetchField()) {
      form_set_error('name', t('The name %name is already taken.', array('%name' => $form_state['values']['name'])));
    }
  }

  // Validate the e-mail address, and check if it is taken by an existing user.
  if ($error = user_validate_mail($form_state['values']['mail'])) {
    form_set_error('mail', $error);
  }
  elseif ((bool) db_select('users')->fields('users', array('uid'))->condition('mail', db_like($form_state['values']['mail']), 'LIKE')->range(0, 1)->execute()->fetchField()) {
    // Format error message dependent on whether the user is logged in or not.
    if ($GLOBALS['user']->uid) {
      form_set_error('mail', t('The e-mail address %email is already taken.', array('%email' => $form_state['values']['mail'])));
    }
    else {
      form_set_error('mail', t('The e-mail address %email is already registered. <a href="@password">Have you forgotten your password?</a>', array('%email' => $form_state['values']['mail'], '@password' => url('user/password'))));
    }
  }
}

/**
 * Submit handler that builds and saves all profiles in the form.
 *
 */
function userreg_school_register_form_submit_handler(&$form, &$form_state) {
 
  $pseudo_entity = (object) $form_state['values'];
  $pseudo_entity->type = 'school';
  
  drupal_set_message('123456');
  $userinfo = array(
    'name' => $form_state['values']['name'],
    'pass' => $form_state['values']['pass'],
    'mail' => $form_state['values']['mail'],
    'init' => $form_state['values']['mail'],
    'status' => 0,
    'login' => REQUEST_TIME
  );
  
  //将用户的角色设置为学校机构用户，这里学校机构的role id为4
  $roles = array(
    2 => 'authenticated user',
    4 => '学校机构用户',
  );
  $userinfo['roles'] = $roles;
  //我们将查询到的信息保存到Drupal7的用户表中
  $account = user_save(drupal_anonymous_user(), $userinfo);
  $pseudo_entity->uid = $account->uid;

  //简体中文
  $pseudo_entity->language = 'zh-hans';
  $pseudo_entity->title = $form_state['values']['title'];
  field_attach_submit('node', $pseudo_entity, $form, $form_state);
  //user_save($user);
  node_save($pseudo_entity);
  $form_state['redirect'] = 'user/companyregister/success';
}

function userreg_school_register_success_page(){
  $output = '';
  $output .= t('您已成功注册学校机构用户。');
  return $output;
}



