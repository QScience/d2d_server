<?php
/**
 * @file
 *
 * implement some vote function based on votingapi framework.
 */

/**
 * used in theme/theme.inc to return a score of current did.
 */
function _d2d_server_get_voting_score($did) {
  $criteria = array(
    'entity_type' => 'd2d_entity',
    'entity_id' => $did,
    'value_type' => 'points',
    'function' => 'sum',
  );
  $vote = votingapi_select_results($criteria);
  if ($vote) {
    return $vote[0]['value'];
  }
  else {
    return 0;
  }
}

/**
 * callback hook_menu() for /d2d/did/voting
 * don't care the votingapi's setting now
 * only insert when the user is not anonymous and 
 * the user hasn't voted before.
 */
function _d2d_server_voting($d2did) {
  $de = d2d_server_get_d2dentity_by_d2did($d2did);
  $did = $de->did;
  $voted = _d2d_server_check_whether_voted($did);
  if ($voted != 0) {
    drupal_goto("d2d/$d2did");
  }
  $votes = array(
    'entity_type' => 'd2d_entity',
    'entity_id' => $did,
    'value_type' => 'points',
    'value' => '1',
  );
  votingapi_add_votes($votes);
  votingapi_recalculate_results('d2d_entity', $did);

  drupal_goto("d2d/$d2did");
}

/**
 * callback hook_menu() for /d2d/did/undovote
 * don't care the votingapi's setting now, just delete the 
 * use's all votes on the d2d instance.
 */
function _d2d_server_undo_vote($d2did) {
  $de = d2d_server_get_d2dentity_by_d2did($d2did);
  $did = $de->did;
  Global $user;
  $votes = db_select('votingapi_vote')
    ->fields('votingapi_vote')
    ->condition('entity_type', 'd2d_entity')
    ->condition('entity_id', $did)
    ->condition('value_type', 'points')
    ->condition('uid', $user->uid)
    ->execute()
    ->fetchAll(PDO::FETCH_ASSOC);
  $vids = array();
  foreach ($votes as $value) {
    $vids[]['vote_id'] = $value['vote_id'];
  }
  votingapi_delete_votes($vids);
  votingapi_recalculate_results('d2d_entity', $did);
  drupal_goto("d2d/$d2did");
}

/**
 * check whether certain user can vote or not.
 * return -1 means this is anonymous.
 * return 0 means the user hasn't voted before.
 * return 1 means the user has voted before.
 */
function _d2d_server_check_whether_voted($did) {
  if (user_is_anonymous()) {
    return -1;
  }
  Global $user;
  $votes = db_select('votingapi_vote')
    ->fields('votingapi_vote')
    ->condition('entity_type', 'd2d_entity')
    ->condition('entity_id', $did)
    ->condition('value_type', 'points')
    ->condition('uid', $user->uid)
    ->execute()
    ->fetchAll(PDO::FETCH_ASSOC);
  if (count($votes) > 0) {
    return 1;
  }
  return 0;
}
