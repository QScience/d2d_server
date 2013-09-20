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


  //use moment.js to format upload time.
  $(".d2d-server-list-table .import-time").text(function(){
    return moment.unix($(this).attr("value")).fromNow();
  });

  //pager ajax functionality.
  $('body').delegate( '.d2d-server-list-table-wrap ul li a', 'click', function(e) {
    var url = $(this).attr('href');
    var current_table = $(this).closest('.d2d-server-list-table-wrap');
    var index = $(current_table).index('.d2d-server-list-table-wrap');
    $.ajax({
      url: url,
      success: function(data) {
        var current_table_new = $('.d2d-server-list-table-wrap', data).eq(index);
        var current_table_new_fieldset_wrap = $(current_table_new).find('.fieldset-wrapper');
        var current_table_fieldset_wrap = $(current_table).find('.fieldset-wrapper');
        $(current_table_fieldset_wrap).hide();
        $(current_table_fieldset_wrap).html($(current_table_new_fieldset_wrap).html()).show('slow');

        $(".d2d-server-list-table .import-time").text(function(){
          return moment.unix($(this).attr("value")).fromNow();
        });

      }
    });
    return false;
  }); 


});
})(jQuery)
