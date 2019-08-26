import React, { Component } from "react";
import firebase from "./config/fbConfig";

class RegWeb extends Component {
  constructor() {
    super();

    this.state = {
      userData: []
    };
    // this.db = firebase.database().ref("/Employers");
  }

  componentDidMount() {
    let empref = firebase.firestore().collection("users");
    let allusers = empref
      .get()
      .then(snapshot => {
        let newState = [];
        snapshot.forEach(doc => {
          console.log(doc.id, "=>", doc.data());
          newState.push({
            secid: doc.data().secid,
            username: doc.data().username
          });
        });
        this.setState({ userData: newState });
      })
      .catch(err => {
        console.log("Error getting documents", err);
      });
  }

  render() {
    return (
      <div>
        <div>
          <h3>
            {" "}
            <strong>&nbsp;&nbsp;&nbsp;Web Users</strong>
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
            </tr>

            <tbody>
              {this.state.userData.map(emp => {
                if (emp.username != "Admin") {
                  return (
                    <tr>
                      <td color="black">{emp.username}</td>

                      <td>{emp.secid}</td>
                    </tr>
                  );
                }
              })}
            </tbody>
          </table>
        </div>
      </div>
    );
  }
}
export default RegWeb;
