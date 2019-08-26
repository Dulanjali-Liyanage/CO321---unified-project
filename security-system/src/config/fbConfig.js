import firebase from "firebase/app";
import "firebase";
//import "firebase/auth";

var firebaseConfig = {
  apiKey: "AIzaSyDivWaFsJLq41ib9QdB7RncG93Cs2_Rx2I",
  authDomain: "automatic-door-lock.firebaseapp.com",
  databaseURL: "https://automatic-door-lock.firebaseio.com",
  projectId: "automatic-door-lock",
  storageBucket: "",
  messagingSenderId: "552935452182",
  appId: "1:552935452182:web:a60db0316da4d2be"
};
// Initialize Firebase
firebase.initializeApp(firebaseConfig);
firebase.firestore().settings({ timestampsInSnapshots: true });

export default firebase;
