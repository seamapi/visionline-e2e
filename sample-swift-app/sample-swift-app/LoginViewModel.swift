//
//  LoginViewModel.swift
//  sample-swift-app
//
//  Created by Severin Ibarluzea on 1/7/24.
//

import SwiftUI
import SeamSDK

class LoginViewModel: ObservableObject {
    @Published var isLoggedIn = false
    @Published var isLoadingCredentials = false
    @Published var areCredentialsLoaded = false
    @Published var isScanning = false
    @Published var doorUnlockedSuccess = false
    @Published var doorUnlockedFailure = false

    @Published var loginError: String?
    @Published var entrances: [String]?
    @Published var credentials: [String]?

    var seam: SeamApi?
    var isEntranceScanningAvailable: Bool { areCredentialsLoaded }
    var canUnlockNearest: Bool { entrances != nil && entrances!.count > 0 }
    
    
    func login(username: String, password: String) {
        loginError = nil
        if (!password.contains("seam_cst")) {
            loginError = "The password wasn't a Client Session Token, for this demo you have to use a client session token"
            return
        }
        
        seam = SeamApi(authToken: password)
        seam?.setEventDelegate {
            print("Received event: \($0)")
        }
        isLoggedIn = true
        
        Task {
            isLoadingCredentials = true
            let result = await seam?.mobileController.launch(providers: ["assa_abloy"])

            if case .failure(let failure) = result {
                print("We failed to launch: \(failure)")
                isLoggedIn = false
                return
            }

            listCredentials()
        }
    }
    
    func startUnlockByTapping() {
        Task {
            let result = await seam?.mobileController.unlockByTapping.start()
            isScanning = true
        }
    }
    
    func stopUnlockByTapping() {
        Task {  
            await seam?.mobileController.unlockByTapping.stop()
            isScanning = false
        }
    }
    
    func listCredentials() {
        Task {
            let credentialsResult = await seam?.mobileController.credentials.list()
            switch(credentialsResult!) {
            case .success(let newCredentials):
                let credentialStrs = newCredentials.map {
                    return "\($0.providerId):\($0.cardNumber)"
                }
                self.credentials = credentialStrs
                print("New credentials: \(newCredentials)")
            case .failure(let error):
                print("Failed to list credentials: \(error)")
            }
        }
    }
    
    func health() {
        let seam = SeamApi()
        Task {
            print(await seam.health())
        }
    }
}
