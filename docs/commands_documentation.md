| Command | Args[1] | Args[2] | Args[3] | Spalte 5 | Spalte 6 |
| ------- | ------- | ------- | ------- | -------- | -------- |
| JOIN    | #channel1 |         |         |          |          |
| JOIN    | #channel1 | password1 |         |          |          |
| JOIN    | #channel1,&channel2,+channel3,!channel4 | password1,password2 |         |          |          |
| PART    | #channel1 |         |         |          |          |
| PART    | #channel2 | :one reason for leaving the channel |         |          |          |
| PART    | #channel1,&channel2,+channel3,!channel4 |         |         |          |          |
| PART    | #channel1,&channel2,+channel3,!channel4 | :one reason for leaving the channel |         |          |          |
| KICK    | #channel1 | user1   |         |          |          |
| KICK    | #channel1 | user1   | :reason for kicking user out of the channel |          |          |
| KICK    | #channel1,&channel2,+channel3,!channel4 | user1,user2 |         |          |          |
| KICK    | #channel1,&channel2,+channel3,!channel4 | user1,user2 | :reason for kicking user out of the channel |          |          |
| INVITE  | user    | #channel |         |          |          |
| TOPIC   | #channel |         |         |          |          |
| TOPIC   | #channel | :       |         |          |          |
| TOPIC   | #channel | :changed Topic here |         |          |          |
| MODE    | #channel | +o      | user1   |          |          |
| MODE    | #channel | -o      | user1   |          |          |
| MODE    | #channel | +k      | password1 |          |          |
| MODE    | #channel | -k      | password1 |          |          |
| MODE    | #channel | +i      |         |          |          |
| MODE    | #channel | -i      |         |          |          |
| MODE    | #channel | +l      | 10      |          |          |
| MODE    | #channel | -l      |         |          |          |
| MODE    | #channel | +t      |         |          |          |
| MODE    | #channel | -t      |         |          |          |
