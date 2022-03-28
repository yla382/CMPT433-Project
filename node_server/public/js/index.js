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

    $('#turnLeftButton').on('click', function() {
        socket.emit('REQUEST', "MOTOR_LEFT");
        console.log("turn Left")
    });
    $('#turnRightButton').on('click', function() {
        socket.emit('REQUEST', "MOTOR_RIGHT");
        console.log("turn right")
    });
    $('#turnUpButton').on('click', function() {
        socket.emit('REQUEST', "MOTOR_GO");
        console.log("go")
    });
    $('#turnDownButton').on('click', function() {
        socket.emit('REQUEST', "MOTOR_STOP");
        console.log("stop")
    });

    socket.on('commandReply', function(result) {
        const commands = result.split(",");
        if (commands[0] == "update") {
            $("#runTime").text(commands[1]);
            $("#lightLevel").text(commands[2]);
            //$('#turnOffBeatBox').show();
        }
    });

    socket.on('canvas', function(data) {
        const canvas = $("#videostream");
        const context = canvas[0].getContext('2d');
        const image = new Image();
        image.src = "data:image/jpeg;base64,"+data;
        image.onload = function(){
            context.height = image.height;
            context.width = image.width;
            context.drawImage(image,0,0,context.width,context.height);
        }
    });
});