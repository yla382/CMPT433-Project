const express = require('express');
const app = express();
const http = require('http');
const server = http.createServer(app);
const { Server } = require("socket.io");
const io = new Server(server);
const startRouter = require('./routers/startPage.js');
const {SERVER_PORT: port = 3000} = process.env;
const dgram = require('dgram');
const config = require('./config.js');
const child = require('child_process');

app.use('/', startRouter);

io.on('connection', (socket) => {
        console.log('a user connected');

        socket.on('REQUEST', (data) => {
                var client = dgram.createSocket('udp4');
                var buffer = new Buffer(data);

                //Replay request from client to Rover
                client.send(buffer, 0, buffer.length, config.rover_1_port, config.rover_1_ip, function(err, bytes) {
                        if (err)  {
                                throw err;
                        }
                });

                //Listen to request from rover
                client.on('listening', function () {
                        var address = client.address();
                });

                //Listen to request from rover
                // Handle an incoming message over the UDP from rover.
                client.on('message', function (message, remote) {
                        var reply = message.toString('utf8');
                        socket.emit('commandReply', reply);
                        client.close();

                });
        });

        //Create child process to handle video streaming
        let ffmpeg = child.spawn("ffmpeg", [
                "-re", 
                "-y", 
                "-i", 
                "udp://192.168.7.1:1234",
                "-preset",  
                "ultrafast", 
                "-f", 
                "mjpeg", 
                "pipe:1"
                ]);
        
        ffmpeg.on('error', function (err) {
                console.log(err);
                throw err;
        });

        ffmpeg.on('close', function (code) {
                console.log('ffmpeg exited with code ' + code);
        });

        ffmpeg.stderr.on('data', function(data) {
                // Don't remove this
                // Child Process hangs when stderr exceed certain memory
        });

        //Convert raw video frame to string and sent to client
        ffmpeg.stdout.on('data', function (data) {
                var frame = Buffer.from(data).toString('base64');
                io.sockets.emit('canvas',frame);
        });
});

server.listen({ port }, () => {
        console.log(`🚀 Server ready at http://0.0.0.0:${port}`);
});