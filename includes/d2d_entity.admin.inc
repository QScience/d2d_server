<?php
/**
 * @file
 */
module_load_include('inc', 'd2d_server', 'includes/db');

/**
 * Implements ENTITY_TYPE_form for admin UI.
 */
function d2d_entity_form($form, &$form_state, $d2d_entity) {
  $form = array();

  $newfriend;

  if (isset($d2d_entity->is_new) && $d2d_entity->is_new) {
    $newfriends = d2d_server_get_instances_newfriends();
    if (!count($newfriends)) {
      $form['noavaild2d'] = array(
        '#prefix' => "All friend instances already added.<br>Go back to the ",
        '#markup' => l(t('list page'), 'admin/d2d_server'),
        '#suffix' => '.',
      );
      return $form;
    }
    else {
      $header = array(
        'd2did' => t('D2D ID'),
        'title' => t('TITLE'),
        'description' => t('DESCRIPTION'),
        'url' => t('URL'),
      );

      $options = array();
      $default_v = 1;
      $i = 1;
      foreach ($newfriends as $key => $value) {
        $instance = d2d_server_get_instance_by_d2did($key);
        $options[$key] = array(
          'd2did' => $instance->d2d_id,
          'title' => $instance->name,
          'description' => $instance->description,
          'url' => $instance->url,
        );
        if ($i == 1) {
          $default_v = $key;
          $i++;
        }
      }

      $form['table'] = array(
        '#type' => 'tableselect',
        '#header' => $header,
        '#options' => $options,
        '#multiple' => FALSE,
        '#js_select' => FALSE,
        '#default_value' => $default_v,
        '#ajax' => array(
          'callback' => 'd2d_entity_form_ajax_callback',
          'wrapper' => 'd2d-entity-form-d2d-instance-fieldset',
          'method' => 'replace',
          'effect' => 'fade',
        ),
      );
      if (!empty($form_state['values']['table'])) {
        $newfriend = d2d_server_get_instance_by_d2did($form_state['values']['table']);
        $d2d_entity = new stdClass;
        $d2d_entity->d2did = $newfriend->d2d_id;
        $d2d_entity->title= $newfriend->name;
        $d2d_entity->description = $newfriend->description;
        $d2d_entity->url = $newfriend->url;
        $d2d_entity->site_name = "N/A";
        $d2d_entity->site_slogan = "N/A";
        $d2d_entity->category = 'default';
        $d2d_entity->users_number = -1;
        $d2d_entity->nodes_number= -1;
      }
      else {
        $newfriend = array_shift($newfriends);
        $d2d_entity = new stdClass;
        $d2d_entity->d2did = $newfriend->d2d_id;
        $d2d_entity->title= $newfriend->name;
        $d2d_entity->description = $newfriend->description;
        $d2d_entity->url = $newfriend->url;
        $d2d_entity->site_name = "N/A";
        $d2d_entity->site_slogan = "N/A";
        $d2d_entity->category ='default';
        $d2d_entity->users_number = -1;
        $d2d_entity->nodes_number= -1;
      }
    }
  }

  $form['d2dinstance'] = array(
    '#type' => 'fieldset',
    '#title' => "d2d instance:",
    '#attributes' => array('id' => array('d2d-entity-form-d2d-instance-fieldset')),
  );
  $form['d2dinstance']['d2did'] = array(
    '#type' => 'textfield',
    '#title' => 'D2D ID',
    '#size' => 32,
    '#value' => $d2d_entity->d2did,
    '#disabled' => TRUE,
  );

  $form['d2dinstance']['title'] = array(
    '#type' => 'textfield',
    '#title' => 'D2D INSTANCE TITLE',
    '#value' => $d2d_entity->title,
    '#maxlength' => 100,
  );

  $form['d2dinstance']['description'] = array(
    '#type' => 'textarea',
    '#title' => 'D2D INSTANCE DESCRIPTION',
    '#value' => $d2d_entity->description,
  );

  $form['d2dinstance']['url'] = array(
    '#type' => 'textfield',
    '#title' => 'URL',
    '#value' => $d2d_entity->url,
    '#disabled' => TRUE,
    '#size' => 100,
  );

  $form['d2dinstance']['category'] = array(
    '#type' => 'textfield',
    '#title' => 'CATEGORY',
    '#size' => 32,
    '#value' => $d2d_entity->category,
  );

  $form['d2dinstance']['site_name'] = array(
    '#type' => 'textfield',
    '#title' => 'WEBSITE NAME',
    '#value' => $d2d_entity->site_name,
    '#maxlength' => 100,
    '#disabled' => TRUE,
  );

  $form['d2dinstance']['site_slogan'] = array(
    '#type' => 'textfield',
    '#title' => 'WEBSITE SLOGAN',
    '#value' => $d2d_entity->site_slogan,
    '#maxlength' => 100,
    '#disabled' => TRUE,
  );

  $form['d2dinstance']['users_number'] = array(
    '#type' => 'textfield',
    '#title' => 'USERS NUMBER',
    '#size' => 32,
    '#value' => $d2d_entity->users_number,
    '#disabled' => TRUE,
  );

  $form['d2dinstance']['nodes_number'] = array(
    '#type' => 'textfield',
    '#title' => 'NODES NUMBER',
    '#size' => 32,
    '#value' => $d2d_entity->nodes_number,
    '#disabled' => TRUE,
  );


  field_attach_form('d2d_entity', $d2d_entity, $form['d2dinstance'], $form_state);

  $form['d2dinstance']['submit'] = array(
    '#type' => 'submit',
    '#value' => 'SAVE',
    '#weight' => 20,
  );

  return $form;
}

/**
 * d2d entity form ajax callback
 */
function d2d_entity_form_ajax_callback(&$form, &$form_state) {
  return $form['d2dinstance'];
}

/**
 * validate function for ENTITY_TYPE_form
 */
function d2d_entity_form_validate(&$form, &$form_state) {
  //dsm($form_state);
  $form_state['values']['title'] = $form_state['input']['title'];
  $form_state['values']['description'] = $form_state['input']['description'];
  $form_state['values']['category'] = $form_state['input']['category'];
}

/**
 * submit function for ENTITY_TYPE_form.
 */
function d2d_entity_form_submit(&$form, &$form_state) {

  $d2d_entity = entity_ui_form_submit_build_entity($form, $form_state); $d2d_entity->save(); }
