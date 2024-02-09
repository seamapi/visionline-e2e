//
//  ContentView.swift
//  sample-swift-app
//
//  Created by Severin Ibarluzea on 1/6/24.
//

import SwiftUI

struct LoginView: View {
    
    @ObservedObject var model = LoginViewModel()
    
    @State private var username: String = "";
    @State private var password: String = "";
    
    var entrances: [String] = ["Front Door", "Back Door"]
    var scanning = true
    
    var body: some View {
        VStack {
            Text("Seam Swift Sample App")
            Text("1. Login, in the demo the username is \"user\" and the password is a client session token (use the CLI to make one!). In a real app, you would authenticate a user normally then pass a client session token to the app.\n2. After you're logged in, the phone will load credentials\n3. After credentials are loaded, you can click \"Start unlock by tapping\" to start running the background scan.\n4. Tap phone against a lock that the user has access to to unlock!")
                .font(.footnote)
                .foregroundColor(Color.gray)
            TextField("Username", text: $username)
                .padding(/*@START_MENU_TOKEN@*/.all, 8.0/*@END_MENU_TOKEN@*/)
                .border(/*@START_MENU_TOKEN@*/Color.black/*@END_MENU_TOKEN@*/, width: /*@START_MENU_TOKEN@*/1/*@END_MENU_TOKEN@*/)
            SecureField("Password (Client Session Token for Demo)", text: $password)
                .padding(/*@START_MENU_TOKEN@*/.all, 8.0/*@END_MENU_TOKEN@*/)
                .border(/*@START_MENU_TOKEN@*/Color.black/*@END_MENU_TOKEN@*/, width: /*@START_MENU_TOKEN@*/1/*@END_MENU_TOKEN@*/)
            Button(action: {
                model.login(username: username, password: password)
            }, label: { Text("Login") })
            .disabled(model.isLoggedIn)
            .padding(.bottom, 30.0)
            if (model.loginError != nil) {
                Text(model.loginError ?? "").foregroundColor(Color.red)
            }
            Button(action: {
                model.startUnlockByTapping()
            }, label: {
                Text("Start unlock by tapping")
            }).disabled(model.isScanning || !model.isLoggedIn)
                .padding(.bottom, 30.0)
            Button(action: {
                model.stopUnlockByTapping()
            }, label: {
                Text("Stop unlock by tapping")
            }).disabled(!model.isScanning || !model.isLoggedIn)
                .padding(.bottom, 30.0)
            Button(action: {
                model.listCredentials()
            }, label: {
                Text("Refresh credentials")
            })
            .padding(.bottom, 30.0)
            
            Text("Loaded Credentials").foregroundColor(Color.gray).bold()
            ForEach(model.credentials ?? [], id: \.self, content: {
                Text($0)
            })
            if ((model.credentials ?? []).count == 0) {
                Text("<empty>").foregroundColor(.gray)
            }
            
            Button(action: {
                model.health()
            }, label: {
                Text("Ping seam connect")
            })
            .padding(.bottom, 30.0)
            .padding(.top, 30.0)
            
            
//            Text("Accessible Entrance List").foregroundColor(Color.gray).bold()
//            ForEach(model.entrances ?? [], id: \.self, content: { entrance in
//                Text(entrance)
//            })
//            if ((model.entrances ?? []).count == 0) {
//                Text("<empty>").foregroundColor(.gray)
//            }
//          
//            Spacer()
        }.padding()
    }
}

#Preview {
    LoginView()
}
