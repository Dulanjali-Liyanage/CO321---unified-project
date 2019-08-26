import React, { Component } from "react";
import { HashRouter as Router, Route, Link, NavLink } from "react-router-dom";
import "./Navbar.css";

import AddDoorUsers from "./AddDoorUsers";

class Add extends Component {
  state = {};
  render() {
    return (
      <Router basename="/add">
        <div className="linkside">
          <div className="gap">
            <Link
              to="/add-user-doors"
              className="waves-effect waves-light btn-small"
            >
              Add Users-Doors
            </Link>
          </div>
          <div className="gap">
            <Link
              to="/add-user-doors"
              className="waves-effect waves-light btn-small"
            >
              Add Doors
            </Link>
          </div>
        </div>
        <div className="barside">
          <Route exact path="/add-user-doors" component={AddDoorUsers} />
        </div>
      </Router>
    );
  }
}

export default Add;
