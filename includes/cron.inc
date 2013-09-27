<?php
/**
 * @file
 */

module_load_include('inc', 'd2d_server', 'includes/io');

/**
 */
function d2d_server_cron() {
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'd2d_entity');
  $query->propertyOrderBy('created', 'DESC')
    ->execute();

  $entities;
  if (isset($query->ordered_results) && count($query->ordered_results)) {
    $ids = array();
    foreach ($query->ordered_results as $value) {
      $ids[] = $value->entity_id;
    }
    $entities = entity_load('d2d_entity', $ids);
  }
 
  $queue = DrupalQueue::get("get_users_number");
  foreach ($entities as $entity) {
    $queue->createItem($entity->d2did);
  }
}

/**
 * Implements hook_cron_queue_info()
 */
function d2d_server_cron_queue_info() {
  $queues['get_users_number'] = array(
    'worker callback' => '_d2d_server_fetch_users_number',
    'time' => 20,
  );
  return $queues;
}