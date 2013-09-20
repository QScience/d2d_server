/**
 * @file 
 * d2d server list table js.
 */
(function($){
$(document).ready(function() {

  //voting link ajax.
  $('body').delegate('.d2d-server-list-table a[class*="voting-link-"]', 'click', function () {
    var class_pid = $(this).attr('title');
    var link = $('.' + class_pid);
    $(link).each(function() {
      $(this).parent().children('.voting-link').addClass('voting-link-odd');
    });

    var url = $(this).attr('href');

    $.ajax({
      url: url,
      success: function(data) {
        var $score = $('#d2d-instance-view-page-description #d2d-server-view-page-score', data);
        var $vote = $('#d2d-server-view-page-vote', data);

        //console.log($vote.text());
        //console.log($score.text());
        var link_parent = $(link).parent();
        //console.log(link_parent);
        $(link_parent).prev().html($score.text());
        if ($vote.text() == 'voted'){
          $(link_parent).html($vote.wrap("<div></div>").parent().html());
          $(link_parent).children('.voted-sign').addClass('voting-link-odd');
        }
        else {
          $(link_parent).html($vote.wrap("<div></div>").parent().html());
        }
      }
    });
    return false;
  });



});
})(jQuery)
