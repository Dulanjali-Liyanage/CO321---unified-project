import React, { Component } from "react";
import { HashRouter as Router, Route, Link, NavLink } from "react-router-dom";
import "./Navbar.css";
import RegWeb from "./RegWeb";
import RegDoors from "./RegDoors";
import AccessDoors from "./AccessDoors";
import ComponentToPrint from "./ComponentToPrint";
import ViewDoorUsers from "./ViewDoorUsers";
import History from "./History";

class View extends Component {
  state = {};
  render() {
    return (
      <div>
        <Router basename="/view">
          <div>
            <div className="linkside">
              <div className="gap">
                <Link
                  to="/registered-users-doors"
                  className="waves-effect waves-light btn-small"
                >
                  Registered Users-Doors
                </Link>
              </div>
              <div className="gap">
                <Link
                  to="/registered-users-web"
                  className="waves-effect waves-light btn-small"
                >
                  Registered Users-Web site
                </Link>
              </div>
              <div className="gap">
                <Link
                  to="/access-doors"
                  className="waves-effect waves-light btn-small"
                >
                  Access-Doors
                </Link>
              </div>
              <div className="gap">
                <Link
                  to="/access-history"
                  className="waves-effect waves-light btn-small"
                >
                  Access-History
                </Link>
              </div>
            </div>
            <div className="barside">
              <Route
                exact
                path="/registered-users-doors"
                component={ViewDoorUsers}
              />
              <Route exact path="/registered-users-web" component={RegWeb} />
              <Route exact path="/access-doors" component={ComponentToPrint} />
              <Route exact path="/access-history" component={History} />
            </div>
          </div>
        </Router>
      </div>
    );
  }
}

export default View;
