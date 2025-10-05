const Trip = require("../models/travlr");

const tripsList = async (req, res) => {
  try {
    const trips = await Trip.getAllTrips();
    if (!trips || trips.length === 0) {
      return res.status(404).json({ message: "Trips not found" });
    } else {
      return res.status(200).json(trips);
    }
  } catch (err) {
    return res.status(500).json({ message: "Error fetching trips", error: err });
  }
};

const tripsFindByCode = async (req, res) => {
  try {
    const trip = await Trip.getTripByCode(req.params.tripCode);
    if (!trip) {
      return res.status(404).json({ message: "Trip not found" });
    } else {
      return res.status(200).json(trip);
    }
  } catch (err) {
    return res.status(500).json({ message: "Error fetching trip", error: err });
  }
};

const tripsAddTrip = async (req, res) => {
  try {
    const trip = await Trip.addTrip(req.body);
    if (!trip) {
      return res.status(400).json({ message: "Trip creation failed" });
    } else {
      return res.status(201).json(trip);
    }
  } catch (err) {
    return res.status(400).json({ message: "Trip creation failed", error: err });
  }
};

const tripsUpdateTrip = async (req, res) => {
  try {
    const trip = await Trip.updateTrip(req.params.tripCode, req.body);
    if (!trip) {
      return res.status(400).json({ message: "Trip update failed" });
    } else {
      return res.status(201).json(trip);
    }
  } catch (err) {
    return res.status(400).json({ message: "Trip update failed", error: err });
  }
};

module.exports = {
  tripsList,
  tripsFindByCode,
  tripsAddTrip,
  tripsUpdateTrip,
};
