var express = require('express');
var router = express.Router();
const ctrlTravel = require('../controllers/travel');

/* GET Travel view */
router.get('/', ctrlTravel.travel);

module.exports = router;