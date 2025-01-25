const express = require('express');
const mqtt = require('mqtt');
const path = require('path');
const util = require('util'); //import node.js util (compare what std is for c++?)
const fs = require('fs'); //fs has file-system related functionality
const bodyParser = require('body-parser');
const crypto = require('crypto'); //for generatign random id


const app = express();


const readFile = util.promisify(fs.readFile);
const writeFile = util.promisify(fs.writeFile);

// MQTT variables
const broker = 'mqtt://18.198.188.151';
const subTopic = 'something';
const ledTopic = 'iAmUnique/LED';
const options = {
    port : 21883,
    clientId : 'myHeadExplodes',
    username : 'SmartIoT',
    password: 'SmartIoTMQTT',
}

app.use(bodyParser.json()); //parse json formatted data
function read(filePath = './message.json') {
    return readFile(path.resolve(__dirname, filePath)).then(data => JSON.parse(data));
}

//same as read but writing, (data needs to be stringified as JSON before writing)
function write(data, filePath = './message.json') {
    return writeFile(path.resolve(__dirname, filePath), JSON.stringify(data));
}

// create an MQTT instance
const client = mqtt.connect(broker, options);

// Check that you are connected to MQTT and subscribe to a topic (connect event)
client.on('connect', () => {
    console.log('Connected to MQTT Broker');
    client.subscribe(subTopic, (err) => {
        if (err) console.error('Error subscribing to topic', err); 
        else console.log('Subscribed to topic.');
    });
    client.subscribe(ledTopic, (err) => {
        if (err) console.error('Error subscribing to topic', err); 
        else console.log('Subscribed to topic.');
    });
});


// handle instance where MQTT will not connect (error event)
client.on('error', (err) => {
    if (err.code === 'ECONNREFUSED') console.error('MQTT connection refused :(', err.message);
    else console.error('SOS', err);
});


// Handle when a subscribed message comes in (message event)
app.post('/', async(req, res) => {
        const {topic, msg} = req.body; //extract topic and message from the request body
        const jsonData = JSON.stringify({topic, msg});
        client.publish(topic, jsonData, (err) => { //publish message to MQTT broker
            if (err) {
                console.error('Error publishing message to MQTT broker:', err);
                res.status(500).send(':(');
            }
            else {
                console.log('Message published to MQTT broker:', msg);
                res.status(200).send('Success');
            }
        });
});


// Listen for messages from MQTT broker
client.on('message', async (topic, message) => {
    try {
        console.log('Received message on topic:', topic, message.toString());
        const received = JSON.parse(message.toString());
        const msg = received.msg;
        const id = crypto.randomBytes(3).toString('hex');
        let messages = await read();
        const newMsg = { id, msg };
        console.log('new message:' , newMsg);
        if (Array.isArray(messages)){
            messages.push(newMsg);
            await write(messages);
        }
        else throw new Error('messages is not an array');
    } catch (error) {
        console.error('Error:', error);
    }
});


// Route to serve the home page
app.get('/', (req, res) => {
    res.sendFile(__dirname + '/index.html');
})


// route to serve the JSON array from the file message.json when requested from the home page
app.get('/messages', async (req, res) => {
    try{
        console.log("trying to fetch the messages from file");
        const messages = await read();
        res.json(messages);
    }
    catch (err){
        console.error('Error reading the messages', err);
    }
});


// Route to serve the page to add a message
app.get('/add', (req, res) => {
    console.log("showing the add message-page");
    res.sendFile(__dirname + '/message.html');
});


//Route to show a selected message. Note, it will only show the message as text. No html needed
app.get('/:id', async (req, res) => {
    try {
        const msgId = req.params.id;
        const messages = await read();
        const msg = messages.find(msg => msg.id === msgId);
        res.json(msg);
    }
    catch (err){
        console.error('everything explodes: ', err);
    }
});


// Route to delete a message by id (Already done for you)
app.delete('/:id', async (req, res) => {
    try {
        const messages = await read();
        write(messages.filter(c => c.id !== req.params.id));
        res.sendStatus(200);
    } catch (e) {
        res.sendStatus(200);
    }
});

// listen to the port
app.listen(3000);