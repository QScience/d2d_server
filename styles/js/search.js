/**
 * @file
 */
(function($){
$(document).ready(function() {

  //autofill search box.
  function switch_autofill(text_select) {
    var value_autofill;
    switch (text_select) {
      case 'D2DID': value_autofill = 'for example: DCE7C5BD11D5BDE422373FE711788EC9'; break;
      case 'Title': value_autofill = 'for example: epf'; break;
      case 'Category': value_autofill = 'for example: default'; break;
      case 'Tags': value_autofill = 'for example: econo'; break;
      default: value_autofill = 'search...';
    }
    return value_autofill;
  }
  var value_input = $('#d2d-server-search input#edit-search').val();
  var value_autofill = '';
  var value_default= '';
  var autofill_sign = false;
  if (value_input.length == 0 ) {
    var text_select = $("select#edit-selected option:selected").text();
    value_default = switch_autofill(text_select);
    $('#d2d-server-search input#edit-search').autofill({
      value: value_default,
      defaultTextColor: '#666',
      activeTextColor: '#333',
    });
    autofill_sign = true;
  }
  $('#d2d-server-search input#edit-search').blur(function() {
    var text_select = $("select#edit-selected option:selected").text();
    value_autofill = switch_autofill(text_select);
    value_input = $(this).val();
    if (value_input == value_default) {
      $(this).autofill({
        value: value_autofill,
        defaultTextColor: '#666',
        activeTextColor: '#333',
      });
      autofill_sign = true;
    }
    else {
      autofill_sign = false;
    }
  });
  $("select#edit-selected").change(function () {
    $("select#edit-selected option:selected").each(function () {
      if ( autofill_sign ) {
        var text_select = $(this).text();
        value_autofill = switch_autofill(text_select);
        $('#d2d-server-search input#edit-search').autofill({
          value: value_autofill,
          defaultTextColor: '#666',
          activeTextColor: '#333',
        });
      }
    });
  })

  //search box width.
  $('input#edit-search').css('width', '65%');

  //search functionality ajax implementation
  $('#d2d-server-search #edit-submit').bind('click', function() {
    var table_wrap = $('#d2d-server-all-table-wrap');
    var page_title = $('#page-title');

    var text_select = $("select#edit-selected option:selected").text();
    var value_default = switch_autofill(text_select);
    var search_text = $('#d2d-server-search #edit-search').val().trim();
    if ( search_text == '' || search_text == value_default) {
      $(table_wrap).hide();
      $(table_wrap).html('To begin the search, type something, for example "block"').show('slow');
    }
    else {
      var search_type = $('#d2d-server-search #edit-selected option:selected').val();
      var path = $(this).attr('name');
      path += search_type + '/' + encodeURIComponent(search_text);
      //console.log(path); 
      //console.log(encodeURIComponent(search_text)); 
      $.ajax({
        url: path,
        success: function(data) {
          //console.log(data);
          var table = $('#d2d-server-all-table-wrap', data).html();
          var page_title_new = $('#page-title', data).html();

          $(page_title).html(page_title_new);
          $(table_wrap).hide();
          $(table_wrap).html(table).slideDown('slow');

          $(".d2d-server-list-table .import-time").text(function(){
            return moment.unix($(this).attr("value")).fromNow();
          });
        }
      });
    }
    return false;
  });

});
})(jQuery)
