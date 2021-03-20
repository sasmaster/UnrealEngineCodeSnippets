

/**
* This method grabs an array of actors,then per actor:
* 1. performs frustum (visibility) test.
* 2. If in frustum - calculates screen space bounds from world AABB
*  Can be used to visualize 2D bounds
*/
inline void CalcFrustumAndGenerateScreenBounds(TArray<AActor*> &ActorsArr)
{
	ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	check(LocalPlayer != nullptr);

	APlayerController* playerController = LocalPlayer->GetPlayerController(GetWorld());
	check(playerController != nullptr);

 
	FSceneViewProjectionData ProjectionData = {};

	mLocalPlayer->GetProjectionData(LocalPlayer->ViewportClient->Viewport, eSSP_FULL, /*out*/ ProjectionData);

	FConvexVolume frustumVolume;
	const FMatrix viewProjMatr = ProjectionData.ComputeViewProjectionMatrix();

	//constructs frustum planes from VP matrix
	GetViewFrustumBounds(frustumVolume, viewProjMatr, true);

 
	for (auto actor : ActorsArr)//replace ActorsArr with your array of world actors
	{
		FVector orig, extent;
		actor->GetActorBounds(true, orig, extent, true);


		if (frustumVolume.GetBoxIntersectionOutcode(orig, extent).GetInside())
		{
 

			const FVector v0(orig.X + extent.X, orig.Y + extent.Y, orig.Z + extent.Z);

			const FVector v1(orig.X - extent.X, orig.Y + extent.Y, orig.Z + extent.Z);

			const FVector v2(orig.X + extent.X, orig.Y - extent.Y, orig.Z + extent.Z);

			const FVector v3(orig.X + extent.X, orig.Y + extent.Y, orig.Z - extent.Z);

			const FVector v4(orig.X - extent.X, orig.Y - extent.Y, orig.Z + extent.Z);

			const FVector v5(orig.X + extent.X, orig.Y - extent.Y, orig.Z - extent.Z);

			const FVector v6(orig.X - extent.X, orig.Y - extent.Y, orig.Z - extent.Z);

			const FVector v7(orig.X - extent.X, orig.Y + extent.Y, orig.Z - extent.Z);

			const FIntRect& viewRect = ProjectionData.GetConstrainedViewRect();

			FVector2D screenBoundsMin;
			FSceneView::ProjectWorldToScreen(actor->GetActorLocation(), viewRect, viewProjMatr, screenBoundsMin);
			FVector2D screenBoundsMax = screenBoundsMin;


			FVector2D rectVertices[8];
			FSceneView::ProjectWorldToScreen(v0, viewRect, viewProjMatr, rectVertices[0]);
			FSceneView::ProjectWorldToScreen(v1, viewRect, viewProjMatr, rectVertices[1]);
			FSceneView::ProjectWorldToScreen(v2, viewRect, viewProjMatr, rectVertices[2]);
			FSceneView::ProjectWorldToScreen(v3, viewRect, viewProjMatr, rectVertices[3]);
			FSceneView::ProjectWorldToScreen(v4, viewRect, viewProjMatr, rectVertices[4]);
			FSceneView::ProjectWorldToScreen(v5, viewRect, viewProjMatr, rectVertices[5]);
			FSceneView::ProjectWorldToScreen(v6, viewRect, viewProjMatr, rectVertices[6]);
			FSceneView::ProjectWorldToScreen(v7, viewRect, viewProjMatr, rectVertices[7]);


			//build screen rectangle bounds (MUST do it vector comps wise and not between vector,it won't expand correctly in the second case)
			for (uint32 i = 0; i < 8; ++i)
			{
				screenBoundsMin.X = FMath::Min(screenBoundsMin.X, rectVertices[i].X);
				screenBoundsMin.Y = FMath::Min(screenBoundsMin.Y, rectVertices[i].Y);

				screenBoundsMax.X = FMath::Max(screenBoundsMax.X, rectVertices[i].X);
				screenBoundsMax.Y = FMath::Max(screenBoundsMax.Y, rectVertices[i].Y);
			}

			const FVector2D screenVerts[2] = { screenBoundsMin ,screenBoundsMax };

           // grab screenVerts and draw lines....Tested on 4.26
		}
 
	}
}