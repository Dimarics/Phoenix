import QtQuick
import QtQuick3D
import QtQuick3D.Helpers
import Components

View3D {
    id: viewport
    signal exit
    anchors.fill: parent
    environment: SceneEnvironment {
        id: sceneEnvironment
        //clearColor: "#002b36"
        clearColor: "#212830"
        backgroundMode: SceneEnvironment.Color
        antialiasingMode: SceneEnvironment.MSAA
        antialiasingQuality: SceneEnvironment.High

        /*backgroundMode: SceneEnvironment.SkyBox
        lightProbe: Texture {
            textureData: ProceduralSkyTextureData {}
        }*/
    }
    // Камера
    camera: cameraNode
    Node {
        id: originNode
        PerspectiveCamera {
            id: cameraNode
            clipNear: 10
            clipFar: 1000000
            fieldOfView: 30
            fieldOfViewOrientation: PerspectiveCamera.Horizontal
            z: 1000
        }
        DirectionalLight {
            //ambientColor: Qt.rgba(0.5, 0.5, 0.5, 1.0)
            brightness: 1.0
            //castsShadow: true
            //shadowMapQuality: Light.ShadowMapQualityHigh
            //shadowBias: 0
            //eulerRotation.x: -25
        }
    }
    /*PointHandler {
        id: pickController
        property var pickResult: null
        onActiveChanged:  {
            if (active) {
                let pick = viewport.pick(point.position.x, point.position.y);
                if (pick.objectHit) {
                    pickResult = pick
                    let joint = pickResult.objectHit;
                    joint.eulerRotation.x += 3
                }
            }
            else {
                pickResult = null
            }
        }
        onPointChanged: console.log(point.position)
    }
    OrbitCameraController {
        states: State {
            when: pickController.pickResult
            PropertyChanges {
                target: cameraController
                xSpeed: 0; ySpeed: 0
            }
        }
        id: cameraController
        origin: originNode
        camera: cameraNode
        //enabled: !pickController.picked
    }*/
    MouseArea {
        id: pickController
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        property real bufferAngle
        property point mousePos
        property vector3d hitPoint
        property Model objectHit
        onPressed: function(mouse) {
            mousePos.x = mouse.x; mousePos.y = mouse.y
            let pickResult = viewport.pick(mouse.x, mouse.y);
            if (pickResult.objectHit) {
                if (pickResult.objectHit.eventFilterJoint) {
                    objectHit = pickResult.objectHit.eventFilterJoint
                    hitPoint = objectHit.mapPositionFromNode(pickResult.objectHit, pickResult.position)
                } else {
                    objectHit = pickResult.objectHit
                    hitPoint = pickResult.position
                }
                bufferAngle = objectHit.jointAngle.value
            }
        }
        onPositionChanged: function(mouse) {
            if (mouse.buttons === Qt.LeftButton) {
                if (objectHit) {
                    const worldPivot = objectHit.mapPositionToScene(objectHit.pivot)
                    const worldRotationAxis = objectHit.mapDirectionToScene(objectHit.rotationAxis)
                    const worldHitPoint = objectHit.mapPositionToScene(hitPoint)
                    const p0 = pointIntersectionWithPlane(worldPivot, worldRotationAxis, worldRotationAxis, worldHitPoint)
                    const v1 = mapToWorld(mousePos.x, mousePos.y, worldRotationAxis, worldHitPoint).minus(p0)
                    const v2 = mapToWorld(mouse.x, mouse.y, worldRotationAxis, worldHitPoint).minus(p0)
                    angleBetweenVectors(v1, v2, worldRotationAxis)
                    /*
                    var angle = angleBetweenVectors(v1, v2, worldRotationAxis) / objectHit.gearRatio
                    bufferAngle += angle
                    objectHit.jointAngle.setValue(bufferAngle)
                    */
                    objectHit.jointAngle.rotate(angleBetweenVectors(v1, v2, worldRotationAxis) / objectHit.gearRatio)
                } else {
                    originNode.eulerRotation.y += (mousePos.x - mouse.x) * 0.1
                    originNode.eulerRotation.x += (mousePos.y - mouse.y) * 0.1
                }
            } else if (mouse.buttons === Qt.RightButton) {
                const v1 = viewport.mapTo3DScene(Qt.vector3d(mousePos.x, mousePos.y, cameraNode.z))
                const v2 = viewport.mapTo3DScene(Qt.vector3d(mouse.x, mouse.y, cameraNode.z))
                originNode.position = originNode.position.plus(v1.minus(v2))
            }
            mousePos.x = mouse.x; mousePos.y = mouse.y
        }
        onReleased: {
            objectHit = null
        }
        onWheel: function(wheel) {
            cameraNode.z *= wheel.angleDelta.y > 0 ? 0.88 : 1.12
        }
        function pointIntersectionWithPlane(rayOrigin: vector3d, rayDir: vector3d,
                                            planeNormal: vector3d, planePoint: vector3d): vector3d {
            const denom = rayDir.dotProduct(planeNormal)
            if (Math.abs(denom) < 0.0001)
                return null
            const t = planeNormal.dotProduct(planePoint.minus(rayOrigin)) / denom

            // если пересечение позади точки выхода луча
            //if (t < 0)
            //    return null

            return rayOrigin.plus(rayDir.times(t))
        }
        function mapToWorld(x, y, planeNormal, planePoint) {
            const rayOrigin = viewport.mapTo3DScene(Qt.vector3d(x, y, 0))
            const rayDir = viewport.mapTo3DScene(Qt.vector3d(x, y, 1)).minus(rayOrigin)
            return pointIntersectionWithPlane(rayOrigin, rayDir, planeNormal, planePoint)
        }
        function angleBetweenVectors(v1: vector3d, v2: vector3d, normal: vector3d): real {
            const angle = Math.atan2(normal.dotProduct(v1.crossProduct(v2)), v1.dotProduct(v2))
            return angle// / Math.PI * 180.0
            //atan2(QVector3D::dotProduct(normal, QVector3D::crossProduct(v1, v2)), QVector3D::dotProduct(v1, v2));
        }
    }
    // Сцена
    /*Model {
        source: "D:/Development/QML_3D/meshes/assimpScene_mesh.mesh"
        y: -104
        eulerRotation.x: -90
        materials: [
            DefaultMaterial {
                diffuseColor: Qt.rgba(0.8, 0.8, 0.8, 1.0)
            }
        ]
    }*/
    Model {
        eulerRotation.x: -90
        //scale: Qt.vector3d(100, 100, 100)
        geometry: GridGeometry {
            horizontalLines: 60
            horizontalStep: 120
            verticalLines: 60
            verticalStep: 120
        }
        materials: PrincipledMaterial {
            baseColor: "gray"
            lighting: PrincipledMaterial.NoLighting
        }
    }
    Button {
        id: exitButton
        toolTip: "Выход"
        //icon.color: Style.buttonHoveredBorderColor
        icon.source: "qrc:/images/exit.svg"
        icon.width: 26; icon.height: 26
        opacity: hovered ? 1 : 0.4
        width: 50; height: 50
        x: parent.width - width - 10; y: 10
        //anchors { verticalCenter: parent.verticalCenter; right: parent.right; margins: 15}
        onClicked: exit()
    }
    /*H1M1 {
        opacity: 0.3
        interactive: false
        //eulerRotation.z: 90
        //eulerRotation.y: 90
    }
    H1M1 {
        //joint_2.userValue: 60
        //eulerRotation.z: 90
        //eulerRotation.y: 90
    }*/
    // Для прозрачных
    //alphaMode: PrincipledMaterial.Blend
    //depthDrawMode: Material.AlwaysDepthDraw
}
