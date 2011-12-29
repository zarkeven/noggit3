#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <string>

class Model;
class Bone;

#include <opengl/types.h>

#include <noggit/Animated.h> // Animation::M2Value
#include <noggit/AsyncObject.h> // AsyncObject
#include <noggit/Manager.h> // ManagedItem
#include <noggit/Matrix.h>
#include <noggit/ModelHeaders.h>
#include <noggit/Particle.h>
#include <noggit/Quaternion.h>
#include <noggit/Vec3D.h>

namespace noggit
{
  class blp_texture;
  namespace mpq
  {
    class file;
  }
}

Vec3D fixCoordSystem(Vec3D v);

class Bone {
  Animation::M2Value<Vec3D> trans;
  Animation::M2Value<Quaternion, PackedQuaternion> rot;
  Animation::M2Value<Vec3D> scale;

public:
  Vec3D pivot, transPivot;
  int parent;

  bool billboard;
  Matrix mat;
  Matrix mrot;

  bool calc;
  void calcMatrix(Bone* allbones, int anim, int time);
  void init(const noggit::mpq::file& f, const ModelBoneDef &b, int *global, noggit::mpq::file **animfiles);

};


class TextureAnim {
  Animation::M2Value<Vec3D> trans, rot, scale;

public:
  Vec3D tval, rval, sval;

  void calc(int anim, int time);
  void init(const noggit::mpq::file& f, const ModelTexAnimDef &mta, int *global);
  void setup(int anim);
};

struct ModelColor {
  Animation::M2Value<Vec3D> color;
  Animation::M2Value<float,int16_t> opacity;

  void init(const noggit::mpq::file& f, const ModelColorDef &mcd, int *global);
};

struct ModelTransparency {
  Animation::M2Value<float,int16_t> trans;

  void init(const noggit::mpq::file& f, const ModelTransDef &mtd, int *global);
};

// copied from the .mdl docs? this might be completely wrong
enum BlendModes {
  BM_OPAQUE,
  BM_TRANSPARENT,
  BM_ALPHA_BLEND,
  BM_ADDITIVE,
  BM_ADDITIVE_ALPHA,
  BM_MODULATE,
  BM_MODULATE2
};

struct ModelRenderPass {
  uint16_t indexStart, indexCount, vertexStart, vertexEnd;
  int tex;
  bool usetex2, useenvmap, cull, trans, unlit, nozwrite, billboard;
  float p;

  int16_t texanim, color, opacity, blendmode, order;

  // Geoset ID
  int geoset;

  // texture wrapping
  bool swrap, twrap;

  // colours
  Vec4D ocol, ecol;

  bool init(Model *m);
  void deinit();

  bool operator< (const ModelRenderPass &m) const
  {
    //return !trans;
    if (order<m.order) return true;
    else if (order>m.order) return false;
    else return blendmode == m.blendmode ? (p<m.p) : blendmode < m.blendmode;
  }
};

struct ModelCamera {
  bool ok;

  Vec3D pos, target;
  float nearclip, farclip, fov;
  Animation::M2Value<Vec3D> tPos, tTarget;
  Animation::M2Value<float> rot;

  void init(const noggit::mpq::file& f, const ModelCameraDef &mcd, int *global);
  void setup(int time=0);

  ModelCamera():ok(false) {}
};

struct ModelLight {
  int type, parent;
  Vec3D pos, tpos, dir, tdir;
  Animation::M2Value<Vec3D> diffColor, ambColor;
  Animation::M2Value<float> diffIntensity, ambIntensity;
  //Animation::M2Value<float> attStart,attEnd;
  //Animation::M2Value<bool> Enabled;

  void init(const noggit::mpq::file&  f, const ModelLightDef &mld, int *global);
  void setup(int time, opengl::light l);
};

class Model: public ManagedItem, public AsyncObject {

  GLuint ModelDrawList;
  GLuint SelectModelDrawList;
  //GLuint TileModeModelDrawList;

  GLuint vbuf, nbuf, tbuf;
  size_t vbufsize;
  bool animated;
  bool animGeometry,animTextures,animBones;
  bool forceAnim;
  noggit::mpq::file **animfiles;

  void init(const noggit::mpq::file& f);


  TextureAnim *texanims;
  ModelAnimation *anims;
  int *globalSequences;
  ModelColor *colors;
  ModelTransparency *transparency;
  ModelLight *lights;
  ParticleSystem *particleSystems;
  RibbonEmitter *ribbons;

  void drawModel( /*bool unlit*/ );
  void drawModelSelect();

  void initCommon(const noggit::mpq::file& f);
  bool isAnimated(const noggit::mpq::file& f);
  void initAnimated(const noggit::mpq::file& f);
  void initStatic(const noggit::mpq::file& f);

  ModelVertex *origVertices;
  Vec3D *vertices, *normals;
  uint16_t *indices;
  size_t nIndices;
  std::vector<ModelRenderPass> passes;

  void animate(int anim);
  void calcBones(int anim, int time);

  void lightsOn(opengl::light lbase);
  void lightsOff(opengl::light lbase);

public:
  std::string _filename; //! \todo ManagedItem already has a name. Use that?
  ModelCamera cam;
  Bone *bones;
  ModelHeader header;

  // ===============================
  // Toggles
  bool *showGeosets;

  // ===============================
  // Texture data
  // ===============================
  std::vector<noggit::blp_texture*> _textures;
  std::vector<std::string> _textureFilenames;
  std::vector<noggit::blp_texture*> _replaceTextures;
  std::vector<int> _specialTextures;
  std::vector<bool> _useReplaceTextures;

  float rad;
  float trans;
  bool animcalc;
  bool mPerInstanceAnimation;
  int anim, animtime;

  Model(const std::string& name, bool forceAnim=false);
  ~Model();
  void draw (bool draw_fog);
  void drawTileMode();
  void drawSelect();
  void updateEmitters(float dt);

  friend struct ModelRenderPass;

  virtual void finishLoading();
};

#endif
