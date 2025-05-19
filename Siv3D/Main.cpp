# include <Siv3D.hpp>

void Main()
{
	Scene::SetBackground(ColorF{ 0.8, 0.9, 1.0 });

	const Texture playerTexture{ U"🚙"_emoji };
	const Texture treeTexture{ U"🌳"_emoji };

	// プレイヤーの X 座標
	double playerPosX = 400;

	// 木の X 座標
	Array<double> trees = { 100, 300, 500, 700, 900 };

	// (400, 300) を中心とする, 拡大率 1.0 倍の, (マウスやキーではなく）プログラムで動かすカメラ
	Camera2D camera{ Vec2{ 400, 300 }, 1.0, CameraControl::None_ };

	double cameraCenterOffset = 0.0;
	double cameraCenterOffsetVelocity = 0.0;

	while (System::Update())
	{
		const double deltaTime = Scene::DeltaTime();

		// カメラの X 座標
		const double cameraPosX = camera.getCenter().x;

		ClearPrint();
		Print << U"playerPosX: {:.1f}"_fmt(playerPosX);
		Print << U"cameraPosX: {:.1f}"_fmt(cameraPosX);

		// 左右キーで移動
		if (KeyLeft.pressed())
		{
			playerPosX -= (200/* * deltaTime*/);
			cameraCenterOffset = Math::SmoothDamp(cameraCenterOffset, 0.0, cameraCenterOffsetVelocity, 0.8);
		}
		else if (KeyRight.pressed())
		{
			playerPosX += (200/* * deltaTime*/);
			cameraCenterOffset = Math::SmoothDamp(cameraCenterOffset, 0.0, cameraCenterOffsetVelocity, 0.8);
		}

		// カメラの目標中心座標を設定する
		camera.setTargetCenter(Vec2{ (playerPosX/* + cameraCenterOffset*/), 300 });

		// カメラを更新する
		camera.update();
		{
			// カメラによる座標変換を適用する
			const auto tr = camera.createTransformer();

			for (const auto& tree : trees)
			{
				// カメラの中心 X 座標と差が 500 ピクセルの物だけを描く（画面外のものを描かない）
				if (AbsDiff(cameraPosX, tree) < 500.0)
				{
					treeTexture.drawAt(tree, 400);
				}
			}

			playerTexture.drawAt(playerPosX, 410);
		}
	}
}
