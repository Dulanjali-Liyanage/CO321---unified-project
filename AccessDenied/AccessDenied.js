import React, { Component } from "react";
import fire from "./fire";
import "./AccessDenied.css";

class AccessDenied extends Component {
  state = {
    accDenied: []
  };

  componentWillMount() {
    this.db = fire.database().ref("/Access_Denied");

    // DataSnapshot
    this.db.on("value", snap => {
      let accDenied = snap.val();
      let newState = [];
      for (let dt in accDenied) {
        newState.push({
          id: dt,
          dt: accDenied[dt]
        });
      }
      this.setState({
        accDenied: newState
      });
    });
  }

  handleSplit(str) {
    const [date, timez] = str.split("T");
    const [time] = timez.split("Z");
    return (
      <div class="alert">
        <span
          class="closebtn"
          onclick="this.parentElement.style.display='none';"
        >
          &times;
        </span>
        <strong><h2>Unauthorized Access!</h2></strong><h1> &nbsp;&nbsp;&nbsp; Date: &nbsp;&nbsp;&nbsp; {date}{" "}
        &nbsp;&nbsp;&nbsp; Time: &nbsp;&nbsp;&nbsp; {time}</h1>
      </div>
    );
  }

  render() {
    return (
      <div>
        {this.state.accDenied.map(rt => {
          return <div>{this.handleSplit(rt.dt)}</div>;
        })}
      </div>
    );
  }
}

export default AccessDenied;
