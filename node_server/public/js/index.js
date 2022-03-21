function openNav() {
    $('.sidebar').css('width', '450px');
    $('.main').css('margin-left', '450px');
    $('#menu-expand').attr('id', 'menu-collapse');
}
  
function closeNav() {
    $('.sidebar').css('width', '65px');
    $('.main').css('margin-left', '65px');
    $('#menu-collapse').attr('id', 'menu-expand');
}

const socket = io();

// socket.on("ROVER_1_AVAILABLE", function() {
//     console.log()
// });

$( document ).ready(function() {
    $('.svg-icon').on('click', function() {
        if($(this).attr('id') === 'menu-collapse') {
            $('.side_Container').children().not('.side_title').hide();
            $('.side_title').children('a').hide();
            closeNav();
        } else {
            openNav();
            $('.side_Container').children().show();
            $('.side_title').children('a').show();
        }
    });


    socket.on("connect", (socket) => {
        console.log("Connected");
    });

    socket.on("disconnect", () => {
        console.log("Disconnected");
      });
});