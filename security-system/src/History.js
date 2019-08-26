import React, { Component } from "react";
import firebase from "./config/fbConfig";

class ComponentToPrint extends Component {
  constructor() {
    super();

    this.state = {
      userhistory: []
    };
    // this.db = firebase.database().ref("/Employers");
  }

  componentDidMount() {
    const empref = firebase.database().ref("/History_Of_Entered");
    empref.on("value", snapshot => {
      // console.log(snapshot.val().enter_date);
      let userhistory = snapshot.val();

      console.log(userhistory);
      let newState = [];
      for (let emp in userhistory) {
        var res = userhistory[emp].split("u");
        newState.push({
          id: emp,

          uidtag: res[0],
          enter_date: res[1],
          enter_time: res[2]
        });
      }
      this.setState({ userhistory: newState });
    });
  }

  render() {
    return (
      <div>
        <div>
          <h3>
            {" "}
            <strong>&nbsp;&nbsp;&nbsp;Door Access</strong>
          </h3>
        </div>
        <div>
          <table class="table table-stripe">
            <tr>
              <th>
                <font color="black">Tag Id</font>
              </th>
              <th>
                <font color="black">Entered Date</font>
              </th>
              <th>
                <font color="black">Entered Time</font>
              </th>
            </tr>

            <tbody>
              {this.state.userhistory.map(emp => {
                return (
                  <tr>
                    <td>{emp.uidtag}</td>
                    <td>{emp.enter_date}</td>
                    <td>{emp.enter_time}</td>
                  </tr>
                );
              })}
            </tbody>
          </table>
        </div>
      </div>
    );
  }
}
export default ComponentToPrint;
