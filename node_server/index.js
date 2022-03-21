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

app.use('/', startRouter);

io.on('connection', (socket) => {
        console.log('a user connected');

        socket.on('REQUEST', (data) => {
                var client = dgram.createSocket('udp4');
                var buffer = new Buffer(data);
                client.send(buffer, 0, buffer.length, config.rover_1_port, config.rover_1_ip, function(err, bytes) {
                        if (err)  {
                                throw err;
                        }
                        //console.log('UDP message sent to ' + HOST +':'+ PORT);
                }) ;


                client.on('listening', function () {
                        var address = client.address();
                });

                // Handle an incoming message over the UDP from the local application.
                client.on('message', function (message, remote) {
                        var reply = message.toString('utf8');
                        socket.emit('commandReply', reply);

                        client.close();

                });
        });
});

server.listen({ port }, () => {
        console.log(`🚀 Server ready at http://0.0.0.0:${port}`);
});