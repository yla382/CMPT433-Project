const express = require('express');
const router = express.Router();
const path = require('path');
const filePath = "/../public/";

router.get('/', function(req, res) {
	res.sendFile(path.join(__dirname + filePath + "html/index.html"));
});

router.get('/index.css', function(req, res) {
	res.sendFile(path.join(__dirname + filePath + "css/index.css"));
});

router.get('/index.js', function(req, res) {
	res.sendFile(path.join(__dirname + filePath + "js/index.js"));
});

router.get('/rover1.jpg', function(req, res) {
	res.sendFile(path.join(__dirname + filePath + "img/rover1.jpg"));
});

module.exports = router;