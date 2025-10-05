const mongoose = require("mongoose");
const Trip = require("../models/travlr"); // register model
const Model = mongoose.model("trips");

// GET: /trips - list all the trips
// regardless of outcome, respose must include HTML status code
// and json message to the requesting client
const tripsList = async (req, res) => {
  const q = await Model.find({}) // no filter, return all records
    .exec();

  // uncomment the following line to show the results of query
  // on the console
  // console.log(q);

  if (!q) {
    // database returned no results
    return res
      .status(404) // 404 Not Found
      .json({ message: "Trips not found" });
  } else {
    // return resulting triplist
    return res
      .status(200) // 200 OK
      .json(q);
  }
};

// GET: /trips/:tripCode - get a specific trip by code
// Regardless of outcome, response must include HTML status code
// and json message to the requesting client
const tripsFindByCode = async (req, res) => {
  const q = await Model.findOne({ code: req.params.tripCode }) // filter by trip code
    .exec();

  // uncomment the following line to show the results of query
  // console.log(q);

  if (!q) {
    // database returned no results
    return res
      .status(404) // 404 Not Found
      .json({ message: "Trip not found" });
  } else {
    // return resulting trip
    return res
      .status(200) // 200 OK
      .json(q);
  }
};

// POST: /trips - Adds a new trip
// Regardless of outcome, response must include HTML status code
// and json message to the requesting client
const tripsAddTrip = async (req, res) => {
  const newTrip = new Trip({
    code: req.body.code,
    name: req.body.name,
    length: req.body.length,
    start: req.body.start,
    resort: req.body.resort,
    perPerson: req.body.perPerson,
    image: req.body.image,
    description: req.body.description,
  });

  const q = await newTrip.save();

  if (!q) {
    // database returned no data
    return res.status(400).json(err);
  } else {
    // return new trip
    return res.status(201).json(q);
  }
};

// PUT: /trips/:tripCode - Adds a new Trip
// Regardless of outcome, response must include HTML status code
// and JSON message to the requesting client
const tripsUpdateTrip = async (req, res) => {
  // Uncomment for debugging
  console.log(req.params);
  console.log(req.body);

  const q = await Model.findOneAndUpdate(
    { code: req.params.tripCode },
    {
      code: req.body.code,
      name: req.body.name,
      length: req.body.length,
      start: req.body.start,
      resort: req.body.resort,
      perPerson: req.body.perPerson,
      image: req.body.image,
      description: req.body.description,
    }
  ).exec();

  if (!q) {
    // Database returned no data
    return res.status(400).json(err);
  } else {
    // Return resulting updated trip
    return res.status(201).json(q);
  }

  // Uncomment the following line to show results of
  operation;
  // on the console
  // console.log(q);
};

module.exports = {
  tripsList,
  tripsFindByCode,
  tripsAddTrip,
  tripsUpdateTrip,
};
