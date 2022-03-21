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

socket.on("connect", (socket) => {
    console.log("Connected");
});

setInterval(function() {
    socket.emit('REQUEST', "UPDATE");
    
}, 900);

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


    $('#turnoffButton').on('click', function() {
        socket.emit('REQUEST', "QUIT");
        alert("Rover #1 off");
        $(this).hide();
    });

    socket.on('commandReply', function(result) {
        const commands = result.split(",");
        if (commands[0] == "update") {
            $("#runTime").text(commands[1]);
            //$('#turnOffBeatBox').show();
        }
    });
});