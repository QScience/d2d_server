<?php
/**
 * @file
 */

/**
 */
function d2d_fetchClientInfo_form($form, &$form_state) {
  $last_fetch_time = variable_get('D2D_LAST_FETCH_TIME', -1);
  if ($last_fetch_time == -1) {
    $last_fetch_time = "never fetch before.";
  }
  $form['fetchbefore'] = array(
    '#markup' => $last_fetch_time . '<br>',
    '#suffix' => l(t('fetch now?'), "d2d_server_homepage"),
  );
  return $form;
}
