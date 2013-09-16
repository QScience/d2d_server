<?php
/**
 * @file
 */
module_load_include('inc', 'd2d_server', 'includes/db');

/**
 * Implements ENTITY_TYPE_form for admin UI.
 */
function d2d_entity_form($form, &$form_state, $d2d_entity) {
  $newfriends = d2d_server_get_instances_newfriends();

  $header = array(
    'd2did' => t('D2D ID'),
    'title' => t('TITLE'),
    'description' => t('DESCRIPTION'),
    'url' => t('URL'),
    'category' => t('CATEGORY'),
  );

  $options = array();
  foreach ($newfriends as $key => $value) {
    $instance = d2d_server_get_instance_by_d2did($key);
    $options[$key] = array(
      'd2did' => $instance->d2d_id,
      'title' => $instance->name,
      'description' => $instance->description,
      'url' => $instance->url,
      'category' => array(
        'data' => array(
          '#type' => 'textfield',
          '#title' => 'hello' . $instance->d2d_id,
          '#title_display' => 'invisible',
          '#value' => 'set later',
          '#name' => "category[$instance->d2d_id]",
        ),
      ),
    );
  }
  $form['table'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
  );
  $form['category'] = array('#type' => 'value');
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Import'),
  );

  return $form;
}

/**
 * validate function for ENTITY_TYPE_form
 */
function d2d_entity_form_validate(&$form, &$form_state) {
}

/**
 * submit function for ENTITY_TYPE_form.
 */
function d2d_entity_form_submit(&$form, &$form_state) {

  $op = $form_state['values']['table'];
  $category = $form_state['values']['category'];
  $en = new stdClass;
  foreach ($op as $key => $value) {
    if ($value === $key) {
      $q = d2d_server_get_instance_by_d2did($key);
      $en->d2did = $key;
      $en->title = $q->name;
      $en->description = $q->description;
      $en->category = $category[$key];
      $en->users_number = -1;

      //$en['d2did'] = $key;
      //$en['title'] = $q->name;
      //$en['description'] = $q->description;
      //$en['category'] = $category;
      //$en['users_number'] = -1;

      entity_save('d2d_entity', $en);
      //d2d_server_import_instance($key, $category[$key]);
    }
  }

}

/**
 * for edit button.
 */
function d2d_entity_edit_form($form, &$form_state, $d2d_entity) {
  dsm($d2d_entity);
  $form['d2did'] = array(
    '#type' => 'textfield',
    '#title' => 'D2D ID',
    '#default_value' => $d2d_entity->d2did,
    '#disabled' => TRUE,
  );
  
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => 'WEBSITE TITLE',
    '#default_value' => $d2d_entity->title,
  );

  $form['description'] = array(
    '#type' => 'textfield',
    '#title' => 'WEBSITE DESCRIPTION',
    '#default_value' => $d2d_entity->description,
  );

  $form['category'] = array(
    '#type' => 'textfield',
    '#title' => 'CATEGORY',
    '#default_value' => $d2d_entity->category,
  );

  $form['users_number'] = array(
    '#type' => 'textfield',
    '#title' => 'USERS NUMBER',
    '#default_value' => $d2d_entity->users_number,
    '#disabled' => TRUE,
  );

  field_attach_form('d2d_entity', $d2d_entity, $form, $form_state);

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'SAVE',
    '#weight' => 20,
  );
  return $form;
}

/**
 * validate for form d2d_entity_edit_form.
 */
function d2d_entity_edit_form_validate($form, &$form_state) {
}

/**
 * submit for form d2d_entity_edit_form.
 */
function d2d_entity_edit_form_submit($form, &$form_state) {
  $op = $form_state['values'];
  dsm($op);
  $en = array();
  foreach ($op as $key => $value) {
    $en['d2did'] = $op['d2did'];
    $en['title'] = $op['title'];
    $en['description'] = $op['description'];
    $en['category'] = $op['category'];
    $en['users_number'] = $op['users_number'];

    d2d_server_update_instance($en);
  }

  $entity = array('bundle' => 'd2d_entity', 'id' => 4);

  //$fields = field_info_instances('d2d_entity', 'd2d_entity');
  //foreach ($fields as $key => $value) {
  //  $p = $value['und'];
  //}
  field_attach_submit('d2d_server', $entity, $form, $form_state);


}
