import React from "react";

const UsersList = db => {
  return (
    <div>
      {db &&
        db.on(
          "child_added",
          snapshot => {
            console.log(snapshot.val().enter_date);
            return <div>hii dinni</div>;
          },
          function(errorObject) {
            console.log("The read failed: " + errorObject.code);
          }
        )}
    </div>
  );
};

export default UsersList;
