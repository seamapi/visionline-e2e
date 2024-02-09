package main

import (
	"context"
	"errors"
	"fmt"
	"log"
	"time"

	api "github.com/seamapi/go"
	acs "github.com/seamapi/go/acs"
	seamclient "github.com/seamapi/go/client"
	useridentities "github.com/seamapi/go/useridentities"
)

func main() {

	client := seamclient.NewClient(seamclient.WithApiKey("seam_Xb1zvpL6_2z3xkcwtqKNhA7iZzL1otGqm"))

	// Create Connect Webview for seam bridge, assa abloy, visionline
	// ...

	systems, sErr := client.Acs.Systems.List(context.Background(), nil)

	if sErr != nil {
		log.Panic(sErr)
	}

	// Search for Assa Abloy credential service acs system and visionline acs system
	var assaAbloySystem *api.AcsSystem
	var visionlineSystem *api.AcsSystem
	for _, s := range systems.AcsSystems {
		if s.ExternalType == "assa_abloy_credential_service" {
			assaAbloySystem = s
		} else if s.ExternalType == "visionline_system" {
			visionlineSystem = s
		}
	}

	if assaAbloySystem == nil {
		log.Panic(errors.New("No Assa Abloy Credential Service found. Did you make sure to connect an Assa Abloy Credential service using the Connect Webview?"))
	}

	if visionlineSystem == nil {
		log.Panic(errors.New("No Visionline System found. Did you make sure to connect a Visionline Acs System using the Connect Webview?"))
	}

	// Create User Identity: This creates a seam user identity resource that can be used to tie resources to a B-line user
	janeEmail := "jane@example.com"
	userIdentityResponse, uErr := client.UserIdentities.Create(context.Background(), &api.UserIdentitiesCreateRequest{
		EmailAddress: &janeEmail,
		// UserIdentityKey: B-line unique user identifier,
	})

	if uErr != nil {
		log.Panic(uErr)
	}

	// Launch Enrollment Automation for User Identity: This tells Seam to automatically handle Assa Abloy Phone invitations for the user from the Mobile SDK.
	shouldCreateCredentialUser := true
	client.UserIdentities.EnrollmentAutomations.Launch(context.Background(), &useridentities.EnrollmentAutomationsLaunchRequest{
		UserIdentityId:               userIdentityResponse.UserIdentity.UserIdentityId,
		CredentialManagerAcsSystemId: assaAbloySystem.AcsSystemId,
		CreateCredentialManagerUser:  &shouldCreateCredentialUser,
	})

	// Create Visionline Acs User: Creates an acs user on the actual physical visionline system tied with the given UserIdentityId
	fullName := "First Last"
	visionlineUserResponse, vErr := client.Acs.Users.Create(context.Background(), &acs.UsersCreateRequest{
		FullName:       &fullName,
		UserIdentityId: &userIdentityResponse.UserIdentity.UserIdentityId,
		AcsSystemId:    visionlineSystem.AcsSystemId,
	})

	if vErr != nil {
		log.Panic(vErr)
	}

	// Create Visionline Mobile Credential: Creates a mobile key which will be provisioned to the Mobile Sdk
	isMultiPhoneSyncCredential := true
	endsAt := time.Now().Add(7 * 24 * time.Hour).Format("2006-01-02T15:04:00Z")
	isOverrideKey := true
	_, cErr := client.Acs.Credentials.Create(context.Background(), &acs.CredentialsCreateRequest{
		AcsUserId:                  visionlineUserResponse.AcsUser.AcsUserId,
		AccessMethod:               "mobile_key",
		IsMultiPhoneSyncCredential: &isMultiPhoneSyncCredential,
		EndsAt:                     &endsAt,
		VisionlineMetadata: &acs.CredentialsCreateRequestVisionlineMetadata{
			CardFormat:    acs.CredentialsCreateRequestVisionlineMetadataCardFormatRfid48.Ptr(),
			IsOverrideKey: &isOverrideKey,
		},
	})

	if cErr != nil {
		log.Panic(cErr)
	}

	// List all entrances and gets the first available BLE entrance to grant access to the user
	// You can grant users to any entrance, but the user won't get access until a credential with the appropriate
	// type has been provisioned
	acsEntrancesResponse, eErr := client.Acs.Entrances.List(context.Background(), nil)

	if eErr != nil {
		log.Panic(eErr)
	}

	// Search for ble entrance by checking profile from entrance visionline_metadata and grant user access to entrance
	for _, entrance := range acsEntrancesResponse.AcsEntrances {
		entranceProfiles := entrance.VisionlineMetadata.Profiles

		isBleEntrance := false
		for _, profile := range entranceProfiles {
			if profile.VisionlineDoorProfileType == "BLE" {
				isBleEntrance = true
			}
		}

		if isBleEntrance {
			_, grantErr := client.Acs.Entrances.GrantAccess(context.Background(), &acs.EntrancesGrantAccessRequest{
				AcsUserId:     visionlineUserResponse.AcsUser.AcsUserId,
				AcsEntranceId: entrance.AcsEntranceId,
			})

			if grantErr != nil {
				log.Panic(grantErr)
			}
		}
	}

	// Create client session to be used on the mobilesdk to register the phone and receive bluetooth mobile key
	clientSessionRes, createSessionErr := client.ClientSessions.Create(context.Background(), &api.ClientSessionsCreateRequest{
		UserIdentityIds: []string{userIdentityResponse.UserIdentity.UserIdentityId},
	})

	if createSessionErr != nil {
		log.Panic(createSessionErr)
	}

	fmt.Println("Created new user session: ", clientSessionRes)
}
