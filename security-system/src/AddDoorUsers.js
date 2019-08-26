import React, { Component } from "react";
import "./UserSignUpForm.css";
import firebase from "./config/fbConfig";

class AddDoorUsers extends Component {
  state = {};

  constructor(props) {
    super(props);

    this.state = {
      uidtag: "",
      username: "",
      secid: "",
      enValue: "not entered"
    };

    this.handleUID = this.handleUID.bind(this);
    this.handleUserName = this.handleUserName.bind(this);
    this.handleSecId = this.handleSecId.bind(this);
    this.handleSubmit = this.handleSubmit.bind(this);
  }

  handleUID = e => {
    this.setState({ uidtag: e.target.value });
    console.log(this.state.uidtag);
  };

  handleUserName = e => {
    this.setState({ username: e.target.value });
  };

  handleSecId = e => {
    this.setState({ secid: e.target.value });
  };

  handleSubmit() {
    this.db = firebase.database().ref("/Employers");

    this.db.push().set({
      uidtag: this.state.uidtag,
      username: this.state.username,
      secid: this.state.secid,
      enValue: this.state.enValue
    });

    this.setState({
      uidtag: "",
      username: "",
      secid: "",
      enValue: ""
    });
  }

  render() {
    return (
      <div class="container-fluid">
        <div class="row no-gutter">
          <div class="d-none d-md-flex col-md-4 col-lg-6 bg-image" />
          <div class="col-md-8 col-lg-6">
            <div class="login d-flex align-items-center py-5">
              <div class="container">
                <div class="row">
                  <div class="col-md-9 col-lg-8 mx-auto">
                    <h3 class="login-heading mb-4">
                      Users for Entries Registration
                    </h3>

                    <div class="input-field">
                      <input
                        type="text"
                        id="uid"
                        class="form-control"
                        placeholder="UID Tag"
                        onChange={this.handleUID}
                      />
                    </div>
                    <div class="input-field">
                      <input
                        type="text"
                        id="username"
                        class="form-control"
                        placeholder="User Name"
                        onChange={this.handleUserName}
                      />
                    </div>

                    <div class="input-field">
                      <input
                        type="text"
                        id="secid"
                        class="form-control"
                        placeholder="Security ID"
                        onChange={this.handleSecId}
                      />
                    </div>

                    <button
                      class="btn btn-lg btn-primary btn-block btn-login text-uppercase font-weight-bold mb-2"
                      onClick={this.handleSubmit}
                    >
                      Register
                    </button>
                  </div>
                </div>
              </div>
            </div>
          </div>
        </div>
      </div>
    );
  }
}

export default AddDoorUsers;
