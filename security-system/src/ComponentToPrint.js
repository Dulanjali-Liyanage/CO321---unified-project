import React, { Component } from "react";
import firebase from "./config/fbConfig";

class ComponentToPrint extends Component {
  constructor() {
    super();

    this.state = {
      emps: []
    };
    // this.db = firebase.database().ref("/Employers");
  }

  componentDidMount() {
    const empref = firebase.database().ref("/Employers");
    empref.on("value", snapshot => {
      // console.log(snapshot.val().enter_date);
      let emps = snapshot.val();
      console.log(emps);
      let newState = [];
      for (let emp in emps) {
        newState.push({
          id: emp,
          enter_date: emps[emp].enter_date,
          enter_time: emps[emp].enter_time,
          secid: emps[emp].secid,
          uidtag: emps[emp].uidtag,
          username: emps[emp].username
        });
      }
      this.setState({ emps: newState });
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
                <font color="black">User Name</font>
              </th>
              <th>
                <font color="black">Security Id</font>
              </th>
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
              {this.state.emps.map(emp => {
                return (
                  <tr>
                    <td>{emp.username}</td>

                    <td>{emp.secid}</td>
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
