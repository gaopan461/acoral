config = {
	TestRadio1 = {
		ConfType = "Radio",
		Confs = {
			{Name = "测试1", Value = 0, Checked = true,},
			{Name = "测试2", Value = 1,},
			{Name = "-------------------gaopan--------------------", CtrlType = "Static",},
			{Name = "测试3", Value = 2,},
		},
	},
	TestRadio2 = {
		ConfType = "RadioWithArg",
		Confs = {
			{Name = "测试1", Value = 0, Checked = true,},
			{Name = "测试2", Value = 1,},
			{Name = "-------------------gaopan--------------------", CtrlType = "Static",},
			{
				Name = "测试3",
				Value = 2,
				Params = {
					{
						CtrlType = "Edit",
						Name = "参数1",
						CName = "param1",
						Default = "100",
						Note = "参数1为整数",
						Cast = "Integer",
					},
					{
						CtrlType = "Combobox",
						Name = "参数2",
						CName = "param2",
						Confs = {
							{Name = "高攀1", Value = 0,},
							{Name = "高攀2", Value = 1,},
							{Name = "高攀3", Value = 2, Checked = true,},
							{Name = "高攀4", Value = 3,},
							{Name = "高攀5", Value = 4,},
							{Name = "高攀6", Value = 5,},
						},
						Note = "组合框测试，默认值为高攀3",
					},
					{
						CtrlType = "CheckCombo",
						Name = "参数3",
						CName = "param3",
						Confs = {
							{Name = "美艳1", Value = 0,},
							{Name = "美艳2", Value = 1, Checked = true,},
							{Name = "美艳3", Value = 2,},
							{Name = "美艳4", Value = 3, Checked = true,},
						},
						Note = "组合复选框，默认为美艳2和美艳4",
					},
				},
			},
		},
	},
	TestCheck1 = {
		ConfType = "Check",
		Confs = {
			{Name = "测试1", Value = 0, Checked = true,},
			{Name = "测试2", Value = 1,},
			{Name = "-------------------gaopan--------------------", CtrlType = "Static",},
			{Name = "测试3", Value = 2, Checked = true,},
		},
	},
	TestCheck2 = {
		ConfType = "CheckWithArg",
		Confs = {
			{Name = "测试1", Value = 0, Checked = true,},
			{Name = "测试2", Value = 1,},
			{Name = "-------------------gaopan--------------------", CtrlType = "Static",},
			{
				Name = "测试3",
				Value = 2,
				Checked = true,
				Params = {
					{
						CtrlType = "Edit",
						Name = "参数1",
						CName = "param1",
						Default = "100",
						Note = "参数1为整数",
						Cast = "Integer",
					},
					{
						CtrlType = "Combobox",
						Name = "参数2",
						CName = "param2",
						Confs = {
							{Name = "高攀1", Value = 0,},
							{Name = "高攀2", Value = 1,},
							{Name = "高攀3", Value = 2, Checked = true,},
							{Name = "高攀4", Value = 3,},
							{Name = "高攀5", Value = 4,},
							{Name = "高攀6", Value = 5,},
						},
						Note = "组合框测试，默认值为高攀3",
					},
					{
						CtrlType = "CheckCombo",
						Name = "参数3",
						CName = "param3",
						Confs = {
							{Name = "美艳1", Value = 0,},
							{Name = "美艳2", Value = 1, Checked = true,},
							{Name = "美艳3", Value = 2,},
							{Name = "美艳4", Value = 3, Checked = true,},
						},
						Note = "组合复选框，默认为美艳2和美艳4",
					},
				},
			},
		},
	},
}
